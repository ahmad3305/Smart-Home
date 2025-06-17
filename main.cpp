#include "Crow/include/crow.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <include/energyUsage.h>
#include <unordered_map>
#include <include/device.h>
#include <include/admin.h>
#include <src/json.hpp>
#include <include/fan.h>
#include <include/light.h>
#include <include/ac.h>
#include <include/heater.h>
#include <include/doorlock.h>
#include <include/windowlock.h>
#include <include/wifi.h>
#include <include/room.h>
#include <string>
#include <include/user.h>
#include <include/home.h>

using json = nlohmann::json;


bool validateCredentials(const std::string& filepath, const std::string& username, const std::string& password) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filepath << std::endl;
        return false;
    }

    std::string line;
    std::string extractedUsername, extractedPassword;

    while (std::getline(file, line)) {
        line.erase(line.find_last_not_of(" \n\r\t") + 1);

        if (line.rfind("username: ", 0) == 0) {
            extractedUsername = line.substr(10);
        }
        else if (line.rfind("password: ", 0) == 0) {
            extractedPassword = line.substr(10);
        }

        if (!extractedUsername.empty() && !extractedPassword.empty()) {
            if (extractedUsername == username && extractedPassword == password) {
                return true;
            }
            extractedUsername.clear();
            extractedPassword.clear();
        }
    }

    return false;
}

std::string readFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filePath << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void writeFile(const std::string& filePath, const std::string& content) {
    std::ofstream file(filePath, std::ios::app);
    file << content << std::endl;
}

std::string generateId(const std::string& prefix) {
    srand(time(nullptr));
    return prefix + "-" + std::to_string(rand() % 100000);
}


std::string getCurrentUserId(const crow::request& req) {
    auto userIdHdr = req.get_header_value("X-User-Id");
    if (!userIdHdr.empty()) return userIdHdr;
    auto userId = req.url_params.get("userId");
    return userId ? std::string(userId) : "U-00001";
}



inline std::string trim_both(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}


std::map<std::string, User*> allUsers;
std::map<std::string, Home*> allHomes;
std::map<std::string, Room*> allRooms;
std::map<std::string, Device*> allDevices;

int main() {
    crow::SimpleApp app;

    User::loadUsersFromFile("data/users.txt", allUsers);
	Home::loadHomesFromFile("data/homes.txt", allHomes);
	Room::loadRoomsFromFile("data/rooms.txt", allRooms, allHomes);
    Device::loadDevicesFromFile("data/devices.txt", allRooms);


    auto fanFeatures = Fan::loadFanFeaturesFromFile("data/fan.txt");
    for (const auto& [roomId, roomPtr] : allRooms) {
        if (!roomPtr) continue;
        for (Device* device : roomPtr->getDevicesVector()) {
            if (device->getDeviceType() == "Fan") {
                Fan* fan = dynamic_cast<Fan*>(device);
                if (fan && fanFeatures.count(fan->getDeviceId()))
                    fan->setSpeed(fanFeatures[fan->getDeviceId()]->getSpeed());
            }
        }
    }

    // Load Light features
    auto lightFeatures = Light::loadLightFeaturesFromFile("data/light.txt");
    for (const auto& [roomId, roomPtr] : allRooms) {
        if (!roomPtr) continue;
        for (Device* device : roomPtr->getDevicesVector()) {
            if (device->getDeviceType() == "Light") {
                Light* light = dynamic_cast<Light*>(device);
                if (light && lightFeatures.count(light->getDeviceId()))
                    light->setBrightness(lightFeatures[light->getDeviceId()]->getBrightness());
            }
        }
    }

    // Load AC features
    auto acFeatures = AC::loadACFeaturesFromFile("data/ac.txt");
    for (const auto& [roomId, roomPtr] : allRooms) {
        if (!roomPtr) continue;
        for (Device* device : roomPtr->getDevicesVector()) {
            if (device->getDeviceType() == "AC") {
                AC* ac = dynamic_cast<AC*>(device);
                if (ac && acFeatures.count(ac->getDeviceId()))
                    ac->setTemperature(acFeatures[ac->getDeviceId()]->getTemperature());
            }
        }
    }

    // Load Heater features
    auto heaterFeatures = Heater::loadHeaterFeaturesFromFile("data/heater.txt");
    for (const auto& [roomId, roomPtr] : allRooms) {
        if (!roomPtr) continue;
        for (Device* device : roomPtr->getDevicesVector()) {
            if (device->getDeviceType() == "Heater") {
                Heater* heater = dynamic_cast<Heater*>(device);
                if (heater && heaterFeatures.count(heater->getDeviceId()))
                    heater->setTemperature(heaterFeatures[heater->getDeviceId()]->getTemperature());
            }
        }
    }

    auto wifiFeatures = Wifi::loadWifiFeaturesFromFile("data/wifi.txt");
    for (const auto& [roomId, roomPtr] : allRooms) {
        if (!roomPtr) continue;
        for (Device* device : roomPtr->getDevicesVector()) {
            if (device->getDeviceType() == "Wifi") {
                Wifi* wifi = dynamic_cast<Wifi*>(device);
                if (wifi && wifiFeatures.count(wifi->getDeviceId()))
                    wifi->setSSID(wifiFeatures[wifi->getDeviceId()]->getSSID());
            }
        }
    }


    CROW_ROUTE(app, "/admindashboard.html").methods("GET"_method)([]() {
        std::string htmlContent = readFile("frontend/admindashboard.html");
        return crow::response(htmlContent);
        });

    CROW_ROUTE(app, "/login").methods("GET"_method)([]() {
        std::string htmlContent = readFile("frontend/login.html");
        return crow::response(htmlContent);
        });

    CROW_ROUTE(app, "/")
        ([](const crow::request&, crow::response& res) {
        res.redirect("/frontend/login.html");
        res.end();
            });

    // Login route

    CROW_ROUTE(app, "/login").methods("POST"_method)([](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body) {
            std::cerr << "Error: Invalid JSON format in login request." << std::endl;
            crow::json::wvalue responseJson;
            responseJson["success"] = false;
            responseJson["error"] = "Invalid JSON format";
            return crow::response{ 400, responseJson };
        }

        std::string username = body["username"].s();
        std::string password = body["password"].s();

        if (validateCredentials("data/admin.txt", username, password)) {
            crow::json::wvalue responseJson;
            responseJson["success"] = true;
            responseJson["userId"] = "admin";
            responseJson["isAdmin"] = true;
            return crow::response{ responseJson };
        }

        for (const auto& pair : allUsers) {
            User* user = pair.second;
            if (!user) continue;
            if (user->getUsernameValue() == username && user->validatePassword(password)) {
                crow::json::wvalue responseJson;
                responseJson["success"] = true;
                responseJson["userId"] = user->getUserId();
                responseJson["isAdmin"] = false;
                return crow::response{ responseJson };
            }
        }

        crow::json::wvalue responseJson;
        responseJson["success"] = false;
        responseJson["error"] = "Invalid credentials";
        return crow::response{ 401, responseJson };
        });

    // API route to fetch users
    CROW_ROUTE(app, "/api/admin/users").methods("GET"_method)
        ([] {
        std::vector<crow::json::wvalue> users;

        for (const auto& pair : allUsers) {
            User* user = pair.second;
            if (!user) continue;

            crow::json::wvalue userObj;
            userObj["userId"] = user->getUserId();
            userObj["username"] = user->getUsername();
            userObj["name"] = user->getName();
            users.push_back(std::move(userObj));
        }

        crow::json::wvalue response;
        response["users"] = std::move(users);
        return crow::response(response);
            });

    CROW_ROUTE(app, "/api/admin/add-user").methods("POST"_method)
        ([](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body) return crow::response(400, "Invalid JSON");

        std::string username = body["username"].s();
        std::string password = body["password"].s();
        std::string name = body["name"].s();

        std::string userId = generateId("U");

        std::string newUserBlock = "\n"
            "userId: " + userId + "\n"
            "username: " + username + "\n"
            "password: " + password + "\n"
            "name: " + name;
        writeFile("data/users.txt", newUserBlock);
        User::loadUsersFromFile("data/users.txt", allUsers);

        crow::json::wvalue res;
        res["status"] = "User added";
        res["userId"] = userId;
        return crow::response(200, res);
            });

    // API route to fetch homes for a user
    CROW_ROUTE(app, "/api/admin/user/<string>/homes").methods("GET"_method)
        ([](const std::string& userId) {
        std::vector<crow::json::wvalue> homesArray;

        for (const auto& pair : allHomes) {
            Home* home = pair.second;
            if (!home) continue;
            if (home->getOwnerID() == userId) {
                crow::json::wvalue homeObj;
                homeObj["homeId"] = home->getHomeId();
                homeObj["homeName"] = home->getHomeName();
                homesArray.push_back(std::move(homeObj));
            }
        }

        crow::json::wvalue response;
        response["homes"] = std::move(homesArray);
        return crow::response(response);
            });

    // API route to add a home for a user
    CROW_ROUTE(app, "/api/admin/user/<string>/add-home").methods("POST"_method)
        ([](const crow::request& req, const std::string& userId) {
        auto body = crow::json::load(req.body);
        if (!body) return crow::response(400, "Invalid JSON");
        std::string homeName = body["homeName"].s();

        std::string newHomeBlock = "\n"
            "homeId: " + generateId("H") + "\n" +
            "homeName: " + homeName + "\n" +
            "ownerId: " + userId;

        std::ofstream fout("data/homes.txt", std::ios::app);
        fout << newHomeBlock << "\n";
        fout.close();
        Home::loadHomesFromFile("data/homes.txt", allHomes);

        return crow::response(200, R"({"status": "Home added successfully"})");
            });

    // API route to fetch rooms for a home
    CROW_ROUTE(app, "/api/admin/home/<string>/rooms").methods("GET"_method)
        ([](const std::string& homeId) {
        auto itHome = allHomes.find(homeId);
        if (itHome == allHomes.end() || !itHome->second) {
            crow::json::wvalue response;
            response["rooms"] = crow::json::wvalue(crow::json::type::List);
            return crow::response(response);
        }

        Home* home = itHome->second;
        std::vector<crow::json::wvalue> roomsArray;

        int roomCount = home->getRoomCount();
        Room** rooms = home->getRooms();

        for (int i = 0; i < roomCount; ++i) {
            Room* room = rooms[i];
            if (!room) continue;
            crow::json::wvalue roomObj;
            roomObj["roomId"] = room->getRoomId();
            roomObj["roomName"] = room->getRoomName();
            roomsArray.push_back(std::move(roomObj));
        }

        crow::json::wvalue response;
        response["rooms"] = std::move(roomsArray);
        return crow::response(response);
            });

    // API route to add a room to a home
    CROW_ROUTE(app, "/api/admin/home/<string>/add-room").methods("POST"_method)
        ([](const crow::request& req, const std::string& homeId) {
        auto body = crow::json::load(req.body);
        if (!body) return crow::response(400, "Invalid JSON");
        std::string roomName = body["roomName"].s();

        std::ofstream fout("data/rooms.txt", std::ios::app);
        fout << "\n"
            << "roomId: " << generateId("R") << "\n"
            << "roomName: " << roomName << "\n"
            << "homeId: " << homeId << "\n";
        fout.close();

        Room::loadRoomsFromFile("data/rooms.txt", allRooms, allHomes);
        Device::loadDevicesFromFile("data/devices.txt", allRooms);

        return crow::response(200, R"({"status": "Room added successfully"})");
            });

    // API route to fetch devices for a room
    CROW_ROUTE(app, "/api/admin/room/<string>/devices").methods("GET"_method)
        ([](const std::string& roomId) {
        auto it = allRooms.find(roomId);
        if (it == allRooms.end() || !it->second) {
            crow::json::wvalue response;
            response["devices"] = crow::json::wvalue(crow::json::type::List);
            return crow::response(response);
        }
        Room* room = it->second;
        std::vector<crow::json::wvalue> devicesArray;
        for (Device* device : room->getDevicesVector()) {
            crow::json::wvalue obj;
            obj["deviceId"] = device->getDeviceId();
            obj["deviceName"] = device->getDeviceName();
            obj["deviceType"] = device->getDeviceType();
            obj["power"] = device->getPower();
            devicesArray.push_back(std::move(obj));
        }
        crow::json::wvalue response;
        response["devices"] = std::move(devicesArray);
        return crow::response(response);
            });

    // API route to add a device to a room
    CROW_ROUTE(app, "/api/admin/room/<string>/add-device").methods("POST"_method)
        ([](const crow::request& req, const std::string& roomId) {
        auto body = crow::json::load(req.body);
        if (!body) return crow::response(400, "Invalid JSON");
        std::string deviceName = body["deviceName"].s();
        std::string deviceType = body["deviceType"].s();
        std::string power = body["power"].s();

        std::string deviceId = generateId("D");
        std::string newDeviceBlock = "\n"
            "deviceId: " + deviceId + "\n" +
            "deviceName: " + deviceName + "\n" +
            "roomId: " + roomId + "\n" +
            "deviceType: " + deviceType + "\n" +
            "status: 0\n" + "power: " + power;

        std::ofstream fout("data/devices.txt", std::ios::app);
        fout << newDeviceBlock << "\n";
        fout.close();

        Device* device = nullptr;
        if (deviceType == "Fan") {
            device = new Fan(deviceId, deviceName, roomId, 0, std::stod(power));
        }
        else if (deviceType == "Light") {
            device = new Light(deviceId, deviceName, roomId, 0, std::stod(power));
        }
        else if (deviceType == "AC") {
            device = new AC(deviceId, deviceName, roomId, 0, std::stod(power));
        }
        else if (deviceType == "Heater") {
            device = new Heater(deviceId, deviceName, roomId, 0, std::stod(power));
        }
        else if (deviceType == "Wifi") {
            device = new Wifi(deviceId, deviceName, roomId, 0, std::stod(power));
        }

        if (device)
        {
            device->addToFile();
            delete device;
        }

        Device::loadDevicesFromFile("data/devices.txt", allRooms);

        return crow::response(200, R"({"status": "Device added successfully"})");
            });

    CROW_ROUTE(app, "/api/admin/warehouses").methods("GET"_method)
        ([] {
        std::string data = readFile("data/warehouse.txt");
        std::stringstream ss(data);
        std::string line, warehouseId, warehouseName, location;
        std::vector<crow::json::wvalue> warehouses;

        while (getline(ss, line)) {
            line.erase(line.find_last_not_of(" \n\r\t") + 1);
            if (line.rfind("warehouseId:", 0) == 0) warehouseId = line.substr(13);
            else if (line.rfind("warehouseName:", 0) == 0) warehouseName = line.substr(14);
            else if (line.rfind("location:", 0) == 0) {
                location = line.substr(9);
                crow::json::wvalue obj;
                obj["warehouseId"] = warehouseId;
                obj["warehouseName"] = warehouseName;
                obj["location"] = location;
                warehouses.push_back(std::move(obj));
                warehouseId.clear(); warehouseName.clear(); location.clear();
            }
        }
        crow::json::wvalue response;
        response["warehouses"] = std::move(warehouses);
        return crow::response(response);
            });

    CROW_ROUTE(app, "/api/admin/add-warehouse").methods("POST"_method)
        ([](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body) return crow::response(400, "Invalid JSON");
        std::string warehouseName = body["warehouseName"].s();
        std::string location = body["location"].s();
        std::string warehouseId = generateId("W");

        std::string newBlock = "\n"
            "warehouseId: " + warehouseId + "\n"
            "warehouseName: " + warehouseName + "\n"
            "location: " + location;
        writeFile("data/warehouse.txt", newBlock);

        crow::json::wvalue res;
        res["status"] = "Warehouse added";
        res["warehouseId"] = warehouseId;
        return crow::response(200, res);
            });

    CROW_ROUTE(app, "/api/admin/warehouse/<string>/inventories").methods("GET"_method)
        ([](const std::string& warehouseId) {
        std::string data = readFile("data/inventory.txt");

        std::stringstream ss(data);
        std::string line, inventoryId, deviceType, quantity, supplier, currentWarehouseId;
        std::vector<crow::json::wvalue> inventories;

        auto trim = [](std::string& s) {
            s.erase(0, s.find_first_not_of(" \t\r\n"));
            s.erase(s.find_last_not_of(" \t\r\n") + 1);
            };

        int total_blocks = 0, matched_blocks = 0;

        while (getline(ss, line)) {
            trim(line);
            if (line.rfind("inventoryId:", 0) == 0) {
                inventoryId = line.substr(12);
                trim(inventoryId);
            }
            else if (line.rfind("warehouseId:", 0) == 0) {
                currentWarehouseId = line.substr(13);
                trim(currentWarehouseId);
            }
            else if (line.rfind("deviceType:", 0) == 0) {
                deviceType = line.substr(11);
                trim(deviceType);
            }
            else if (line.rfind("quantity:", 0) == 0) {
                quantity = line.substr(9);
                trim(quantity);
            }
            else if (line.rfind("supplier:", 0) == 0) {
                supplier = line.substr(9);
                trim(supplier);
                ++total_blocks;
                if (currentWarehouseId == warehouseId) {
                    crow::json::wvalue obj;
                    obj["inventoryId"] = inventoryId;
                    obj["deviceType"] = deviceType;
                    obj["quantity"] = quantity;
                    obj["supplier"] = supplier;
                    inventories.push_back(std::move(obj));
                    ++matched_blocks;
                }
                inventoryId.clear(); deviceType.clear(); quantity.clear(); supplier.clear(); currentWarehouseId.clear();
            }
        }

        crow::json::wvalue response;
        response["inventories"] = std::move(inventories);
        return crow::response(response);
            });

    CROW_ROUTE(app, "/api/admin/warehouse/<string>/add-inventory").methods("POST"_method)
        ([](const crow::request& req, const std::string& warehouseId) {
        auto body = crow::json::load(req.body);
        if (!body) return crow::response(400, "Invalid JSON");
        std::string deviceType = body["deviceType"].s();
        std::string quantity = body["quantity"].s();
        std::string supplier = body["supplier"].s();
        std::string inventoryId = generateId("I");

        std::string newBlock = "\n"
            "inventoryId: " + inventoryId + "\n"
            "warehouseId: " + warehouseId + "\n"
            "deviceType: " + deviceType + "\n"
            "quantity: " + quantity + "\n"
            "supplier: " + supplier;
        writeFile("data/inventory.txt", newBlock);

        crow::json::wvalue res;
        res["status"] = "Inventory added";
        res["inventoryId"] = inventoryId;
        return crow::response(200, res);
            });

    CROW_ROUTE(app, "/api/admin/workers").methods("GET"_method)
        ([](const crow::request& req) {
        std::string data = readFile("data/workers.txt");
        std::stringstream ss(data);
        std::string line, key, value;
        std::vector<crow::json::wvalue> workers;
        crow::json::wvalue worker;

        while (std::getline(ss, line)) {
            if (line.empty()) continue;
            auto pos = line.find(":");
            if (pos == std::string::npos) continue;
            key = line.substr(0, pos);
            value = line.substr(pos + 1);
            while (!value.empty() && (value[0] == ' ' || value[0] == '\t')) value.erase(0, 1);

            if (key == "workerId") worker["workerId"] = value;
            else if (key == "name") worker["name"] = value;
            else if (key == "contact") worker["contact"] = value;
            else if (key == "skillLevel") worker["skillLevel"] = value;
            else if (key == "deviceType") {
                worker["deviceType"] = value;
                workers.push_back(std::move(worker));
                worker = crow::json::wvalue();
            }
        }
        crow::json::wvalue result;
        result["workers"] = std::move(workers);
        return crow::response(200, result);
            });

    CROW_ROUTE(app, "/api/admin/workers/add").methods("POST"_method)
        ([](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body) return crow::response(400, "Invalid JSON");
        std::string name = body["name"].s();
        std::string contact = body["contact"].s();
        std::string skillLevel = body["skillLevel"].s();
        std::string deviceType = body["deviceType"].s();

        std::string workerId = generateId("W");

        std::string block = "\n"
            "workerId: " + workerId + "\n"
            "name: " + name + "\n"
            "contact: " + contact + "\n"
            "skillLevel: " + skillLevel + "\n"
            "deviceType: " + deviceType;

        writeFile("data/workers.txt", block);

        crow::json::wvalue res;
        res["status"] = "Worker added";
        res["workerId"] = workerId;
        return crow::response(200, res);
            });

    // GET /api/user/homes
    CROW_ROUTE(app, "/api/user/homes").methods("GET"_method)
        ([](const crow::request& req) {
        std::string userId = getCurrentUserId(req);
        std::cout << "userId: " << userId << std::endl;
        crow::json::wvalue result;
        std::vector<crow::json::wvalue> homes_json;

        for (const auto& pair : allHomes) {
            Home* home = pair.second;
            if (!home) continue;
            if (home->getOwnerID() == userId) {
                crow::json::wvalue h;
                h["homeId"] = home->getHomeId();
                h["homeName"] = home->getHomeName();
                homes_json.push_back(std::move(h));
            }
        }
        result["homes"] = std::move(homes_json);
        return crow::response(result);
            });

    CROW_ROUTE(app, "/api/user/home/<string>").methods("GET"_method)
        ([](const crow::request& req, const std::string& homeId) {
        std::string userId = getCurrentUserId(req);

        auto itHome = allHomes.find(homeId);
        if (itHome == allHomes.end() || !itHome->second)
            return crow::response(404);

        Home* home = itHome->second;

        if (home->getOwnerID() != userId)
            return crow::response(403);

        crow::json::wvalue result;
        result["home"]["homeId"] = home->getHomeId();
        result["home"]["homeName"] = home->getHomeName();
        result["home"]["ownerId"] = home->getOwnerID();

        return crow::response(result);
            });
    
    // GET /api/user/home/<string>/rooms

    CROW_ROUTE(app, "/api/user/home/<string>/rooms").methods("GET"_method)
        ([](const crow::request& req, const std::string& homeId) {
        std::string userId = getCurrentUserId(req);

        auto itHome = allHomes.find(homeId);
        if (itHome == allHomes.end() || !itHome->second) return crow::response(404);

        Home* home = itHome->second;
        if (home->getOwnerID() != userId) return crow::response(403);

        crow::json::wvalue result;
        std::vector<crow::json::wvalue> rooms_json;

        int roomCount = home->getRoomCount();
        Room** rooms = home->getRooms();

        for (int i = 0; i < roomCount; ++i) {
            Room* room = rooms[i];
            if (!room) continue;
            crow::json::wvalue r;
            r["roomId"] = room->getRoomId();
            r["roomName"] = room->getRoomName();
            rooms_json.push_back(std::move(r));
        }
        result["rooms"] = std::move(rooms_json);
        return crow::response(result);
            });

    // GET /api/user/room/<string>/devices

    CROW_ROUTE(app, "/api/user/rooms/<string>/devices")
        ([](const std::string& rawRoomId) {
        std::string roomId = trim_both(rawRoomId);

        Room* foundRoom = nullptr;

        for (const auto& pair : allHomes) {
            Home* h = pair.second;
            if (!h) continue;
            Room* r = h->getRoom(roomId);
            if (r) {
                foundRoom = r;
                break;
            }
        }

        if (!foundRoom) {
            auto it = allRooms.find(roomId);
            if (it != allRooms.end()) {
                foundRoom = it->second;
            }
        }

        if (!foundRoom) {
            return crow::response(404, "Room not found");
        }

        nlohmann::json devicesJson = nlohmann::json::array();
        auto devices = foundRoom->getDevicesVector();
        for (Device* device : devices) {
            nlohmann::json dev;
            dev["deviceId"] = device->getDeviceId();
            dev["deviceName"] = device->getDeviceName();
            dev["deviceType"] = device->getDeviceType();
            dev["status"] = device->getStatus();
            dev["power"] = device->getPower();

            std::string type = device->getDeviceType();
            if (type == "Fan") {
                Fan* fan = dynamic_cast<Fan*>(device);
                if (fan) dev["speed"] = fan->getSpeed();
            }
            else if (type == "Light") {
                Light* light = dynamic_cast<Light*>(device);
                if (light) dev["brightness"] = light->getBrightness();
            }
            else if (type == "AC") {
                AC* ac = dynamic_cast<AC*>(device);
                if (ac) dev["temperature"] = ac->getTemperature();
            }
            else if (type == "Heater") {
                Heater* heater = dynamic_cast<Heater*>(device);
                if (heater) dev["temperature"] = heater->getTemperature();
            }
            else if (type == "Wifi") {
                Wifi* wifi = dynamic_cast<Wifi*>(device);
                if (wifi) dev["ssid"] = wifi->getSSID();
            }

            devicesJson.push_back(dev);
        }

        return crow::response(devicesJson.dump());
            });

    CROW_ROUTE(app, "/api/devices/<string>/toggle").methods("POST"_method)
        ([&](const crow::request& req, const std::string& deviceId) {
        Device* device = nullptr;
        for (const auto& [roomId, roomPtr] : allRooms) {
            if (!roomPtr) continue;
            for (Device* d : roomPtr->getDevicesVector()) {
                if (d->getDeviceId() == deviceId) {
                    device = d;
                    break;
                }
            }
            if (device) break;
        }
        if (!device) return crow::response(404, "Device not found");

        bool newStatus = !device->getStatus();
        device->setStatus(newStatus);
        device->updateMainDeviceFile("data/devices.txt");

        const std::string usageFile = "data/energyUsages.txt";
        double deviceRate = device->getPower() / 1000;

        if (newStatus) {
            std::string usageId = generateId("EU");
            EnergyUsage usage(usageId, deviceId);
            usage.setOnTimestamp();
            usage.calculateEnergyConsumed(0);
            usage.calculateCost(0);

            std::ofstream fout(usageFile, std::ios::app);
            {
                std::ifstream fin(usageFile, std::ios::binary);
                if (fin) {
                    fin.seekg(-1, std::ios::end);
                    char last;
                    fin.get(last);
                    if (last != '\n') fout << "\n";
                }
            }
            fout << "\n";
            fout << "usageId: " << usage.getUsageId() << "\n";
            fout << "deviceId: " << usage.getDeviceId() << "\n";
            fout << "onTimestamp: " << EnergyUsage::formatTime(usage.getOnTimestamp()) << "\n";
            fout << "offTimestamp: \n";
            fout << "energyConsumed: 0\n";
            fout << "cost: 0";
        }
        else {
            std::ifstream fin(usageFile);
            std::vector<std::string> lines;
            std::string line;
            std::vector<int> blockStartIndexes;
            int idx = 0;
            while (std::getline(fin, line)) {
                lines.push_back(line);
                if (line.rfind("usageId:", 0) == 0)
                    blockStartIndexes.push_back(idx);
                idx++;
            }
            fin.close();

            int foundIdx = -1;
            std::string usageId, onTs;
            for (int i = (int)blockStartIndexes.size() - 1; i >= 0; --i) {
                int startIdx = blockStartIndexes[i];
                std::string dId, offTs;
                for (int j = 0; j < 6 && (startIdx + j) < (int)lines.size(); ++j) {
                    std::string l = lines[startIdx + j];
                    if (l.rfind("deviceId:", 0) == 0) {
                        dId = l.substr(9);
                        dId.erase(0, dId.find_first_not_of(" \n\r\t"));
                    }
                    if (l.rfind("offTimestamp:", 0) == 0) {
                        offTs = l.substr(13);
                        offTs.erase(0, offTs.find_first_not_of(" \n\r\t"));
                    }
                    if (l.rfind("usageId:", 0) == 0) {
                        usageId = l.substr(8);
                        usageId.erase(0, usageId.find_first_not_of(" \n\r\t"));
                    }
                    if (l.rfind("onTimestamp:", 0) == 0) {
                        onTs = l.substr(12);
                        onTs.erase(0, onTs.find_first_not_of(" \n\r\t"));
                    }
                }
                if (dId == deviceId && (offTs.empty() || offTs.find_first_not_of(" \n\r\t") == std::string::npos)) {
                    foundIdx = startIdx;
                    break;
                }
            }
            if (foundIdx >= 0) {
                EnergyUsage usage(usageId, deviceId);

                std::tm on_tm = {};
                std::istringstream iss(onTs);
                iss >> std::get_time(&on_tm, " %Y-%m-%d %H:%M:%S");
                auto on_time = std::chrono::system_clock::from_time_t(std::mktime(&on_tm));
                usage.setOnTimestamp(on_time);

                constexpr double TIME_SCALE = 60.0;
                using namespace std::chrono;
                auto realDuration = std::chrono::system_clock::now() - usage.getOnTimestamp();
                auto scaledDuration = duration_cast<seconds>(realDuration) * int(TIME_SCALE);
                auto scaledOffTimestamp = usage.getOnTimestamp() + scaledDuration;
                usage.setOffTimestamp(scaledOffTimestamp);

                usage.calculateEnergyConsumed(deviceRate);
                double pricePerKWh = 0.5;
                usage.calculateCost(pricePerKWh);

                for (int j = 0; j < 6 && (foundIdx + j) < (int)lines.size(); ++j) {
                    if (lines[foundIdx + j].rfind("offTimestamp:", 0) == 0)
                        lines[foundIdx + j] = "offTimestamp: " + EnergyUsage::formatTime(usage.getOffTimestamp());
                    if (lines[foundIdx + j].rfind("energyConsumed:", 0) == 0)
                        lines[foundIdx + j] = "energyConsumed: " + std::to_string(usage.getEnergyConsumed());
                    if (lines[foundIdx + j].rfind("cost:", 0) == 0)
                        lines[foundIdx + j] = "cost: " + std::to_string(usage.getCost());
                }
                std::ofstream fout(usageFile, std::ios::trunc);
                for (const auto& l : lines) fout << l << "\n";
            }
        }

        return crow::response(200, "Status updated");
            });

    CROW_ROUTE(app, "/api/user/device/<string>/update").methods("POST"_method)(
    [](const crow::request& req, const std::string& deviceId) {
        auto body = crow::json::load(req.body);
        std::string userId = req.get_header_value("X-User-Id");
        if (!body || userId.empty()) {
            return crow::response(400, R"({"success": false, "error": "Invalid request"})");
        }

        std::string feature = body["feature"].s();
        auto value = body["value"];

        auto it = allDevices.find(deviceId);
        if (it == allDevices.end() || !it->second) {
            return crow::response(404, R"({"success": false, "error": "Device not found"})");
        }
        Device* device = it->second;

        if (feature == "temperature") {
            if (auto ac = dynamic_cast<AC*>(device)) {
                if (value.t() == crow::json::type::Number) {
                    ac->setTemperature(value.i());
                } else {
                    return crow::response(400, R"({"success": false, "error": "Invalid value for temperature"})");
                }
            } else {
                return crow::response(400, R"({"success": false, "error": "Device is not an AC"})");
            }
        } else if (feature == "speed") {
            if (auto fan = dynamic_cast<Fan*>(device)) {
                if (value.t() == crow::json::type::Number) {
                    fan->setSpeed(value.i());
                } else {
                    return crow::response(400, R"({"success": false, "error": "Invalid value for speed"})");
                }
            } else {
                return crow::response(400, R"({"success": false, "error": "Device is not a Fan"})");
            }
        } else if (feature == "brightness") {
            if (auto light = dynamic_cast<Light*>(device)) {
                if (value.t() == crow::json::type::Number) {
                    light->setBrightness(value.i());
                } else {
                    return crow::response(400, R"({"success": false, "error": "Invalid value for brightness"})");
                }
            } else {
                return crow::response(400, R"({"success": false, "error": "Device is not a Light"})");
            }
        } else {
            return crow::response(400, R"({"success": false, "error": "Unknown feature"})");
        }

        crow::json::wvalue response;
        response["success"] = true;
        response["deviceId"] = deviceId;
        response["feature"] = feature;
        response["value"] = value;
        return crow::response{response};
    });

    CROW_ROUTE(app, "/api/device/<string>/energy-summary").methods("GET"_method)
        ([](const crow::request& req, const std::string& deviceId) {
        double totalEnergy = EnergyUsage::totalEnergyUsed(deviceId);
        double totalCost = EnergyUsage::totalCost(deviceId);
        crow::json::wvalue res;
        res["totalEnergyUsed"] = std::round(totalEnergy * 1000.0) / 1000.0;
        res["totalCost"] = std::round(totalCost * 1000.0) / 1000.0;
        return crow::response(200, res);
            });

    CROW_ROUTE(app, "/api/device/<string>/energyusages").methods("GET"_method)
        ([](const crow::request& req, const std::string& deviceId) {
        auto allUsages = EnergyUsage::getAllUsages("data/energyUsages.txt");
        std::vector<crow::json::wvalue> filteredUsages;
        for (const auto& usage : allUsages) {
            if (usage.deviceId == deviceId) {
                crow::json::wvalue u;
                u["usageId"] = usage.usageId;
                u["deviceId"] = usage.deviceId;
                u["onTimestamp"] = usage.formatTime(usage.onTimestamp);
                u["offTimestamp"] = usage.formatTime(usage.offTimestamp);
                u["energyConsumed"] = usage.energyConsumed;
                u["cost"] = usage.cost;
                filteredUsages.push_back(std::move(u));
            }
        }
        crow::json::wvalue res;
        res["usages"] = std::move(filteredUsages);
        return crow::response(200, res);
            });


    CROW_ROUTE(app, "/<path>")
        ([](const crow::request&, crow::response& res, std::string path) {
        std::ifstream in(path.c_str(), std::ifstream::in);
        if (in) {
            std::ostringstream contents;
            contents << in.rdbuf();
            in.close();
            res.code = 200;

            if (path.size() >= 5 && path.substr(path.size() - 5) == ".html") {
                res.set_header("Content-Type", "text/html; charset=utf-8");

            }

            res.write(contents.str());
        }
        else {
            res.code = 404;
            res.write("Not Found");
        }
        res.end();
            });

    app.port(18080).multithreaded().run();
}