#include "../include/room.h"
#include "../include/Device.h"
#include "../include/home.h"
#include <fstream>
#include <iostream>
#include <map>


extern std::map<std::string, Room*> allRooms;
extern std::map<std::string, Home*> allHomes;

static std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

void Room::clearDevices() {
    for (int i = 0; i < deviceCapacity; ++i) {
        devices[i] = nullptr;
    }
    deviceCount = 0;
}

Room::Room()
    : roomId(""), roomName(""), homeId(""), devices(nullptr), deviceCount(0), deviceCapacity(0) {
}

Room::Room(const std::string& roomId, const std::string& roomName, const std::string& _homeId)
    : roomId(roomId), roomName(roomName), homeId(_homeId), devices(nullptr), deviceCount(0), deviceCapacity(0) {
}

Room::~Room() {
    if (devices) {
        delete[] devices;
        devices = nullptr;
    }
}
std::string Room::getRoomId() const { return roomId; }
std::string Room::getRoomName() const { return roomName; }
std::string Room::getHomeId() const { return homeId; }

void Room::setRoomName(const std::string& newName) { roomName = newName; }
void Room::setHomeId(const std::string& newHomeId) { homeId = newHomeId; }

void Room::addDevice(Device* device) {
    if (deviceCount == deviceCapacity) {
        int newCapacity = deviceCapacity == 0 ? 4 : deviceCapacity * 2;
        Device** newDevices = new Device * [newCapacity];
        for (int i = 0; i < deviceCount; ++i)
            newDevices[i] = devices[i];
        delete[] devices;
        devices = newDevices;
        deviceCapacity = newCapacity;
    }
    devices[deviceCount++] = device;
}

void Room::removeDevice(const std::string& deviceId) {
    for (int i = 0; i < deviceCount; ++i) {
        if (devices[i] && devices[i]->getDeviceId() == deviceId) {
            delete devices[i];
            for (int j = i; j < deviceCount - 1; ++j)
                devices[j] = devices[j + 1];
            devices[--deviceCount] = nullptr;
            break;
        }
    }
}

Device* Room::getDevice(const std::string& deviceId) const {
    for (int i = 0; i < deviceCount; ++i)
        if (devices[i] && devices[i]->getDeviceId() == deviceId)
            return devices[i];
    return nullptr;
}

int Room::getDeviceCount() const { return deviceCount; }
Device** Room::getDevices() const { return devices; }

std::vector<Device*> Room::getDevicesVector() const {
    std::vector<Device*> vec;
    for (int i = 0; i < deviceCount; ++i) {
        if (devices[i]) vec.push_back(devices[i]);
    }
    return vec;
}

void Room::loadRoomsFromFile(const std::string& filename, std::map<std::string, Room*>& allRooms, std::map<std::string, Home*>& allHomes) {
    for (auto& kv : allRooms) delete kv.second;
    allRooms.clear();
    for (auto& kv : allHomes)
        if (kv.second) kv.second->clearRooms();

    std::ifstream fin(filename);
    if (!fin) {
        std::cerr << "Error: Cannot open " << filename << std::endl;
        return;
    }
    std::string line, roomId, roomName, homeId;
    while (std::getline(fin, line)) {
        line = trim(line);
        if (line.empty()) continue;
        if (line.rfind("roomId:", 0) == 0) roomId = trim(line.substr(7));
        else if (line.rfind("roomName:", 0) == 0) roomName = trim(line.substr(9));
        else if (line.rfind("homeId:", 0) == 0) {
            homeId = trim(line.substr(7));

            addRoomToHome(roomId, roomName, homeId);

            roomId.clear(); roomName.clear(); homeId.clear();
        }
    }
}

Room* Room::addRoomToHome(const std::string& roomId, const std::string& roomName, const std::string& homeId) {
    Room* room = nullptr;
    auto it = allRooms.find(roomId);
    if (it != allRooms.end()) {
        room = it->second;
    }
    else {
        room = new Room(roomId, roomName, homeId);
        allRooms[roomId] = room;
    }

    auto hit = allHomes.find(homeId);
    if (hit != allHomes.end() && hit->second) {
        hit->second->addRoom(room);
    }
    return room;
}