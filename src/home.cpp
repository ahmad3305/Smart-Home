#include "../include/home.h"
#include "../include/Room.h"
#include <fstream>
#include <iostream>
#include <string>
#include <map>

std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

Home::Home()
    : homeId(""), homeName(""), ownerid(""), rooms(nullptr), roomCount(0), roomCapacity(0) {
}

Home::Home(const std::string& homeId, const std::string& homeName, const std::string& owner)
    : homeId(homeId), homeName(homeName), ownerid(owner), rooms(nullptr), roomCount(0), roomCapacity(0) {
}

Home::~Home() {
    if (rooms) {
        delete[] rooms;
        rooms = nullptr;
    }
}

std::string Home::getHomeId() const { return homeId; }
std::string Home::getHomeName() const { return homeName; }
std::string Home::getOwnerID() const { return ownerid; }

void Home::addRoom(Room* room) {
    if (roomCount == roomCapacity) {
        int newCapacity = roomCapacity == 0 ? 2 : roomCapacity * 2;
        Room** newRooms = new Room * [newCapacity];
        for (int i = 0; i < roomCount; ++i)
            newRooms[i] = rooms[i];
        delete[] rooms;
        rooms = newRooms;
        roomCapacity = newCapacity;
    }
    rooms[roomCount++] = room;
}

void Home::removeRoom(const std::string& roomId) {
    for (int i = 0; i < roomCount; ++i) {
        if (rooms[i]->getRoomId() == roomId) {
            for (int j = i; j < roomCount - 1; ++j)
                rooms[j] = rooms[j + 1];
            --roomCount;
            return;
        }
    }
}

Room* Home::getRoom(const std::string& roomId) const {
    for (int i = 0; i < roomCount; ++i) {
        if (rooms[i]->getRoomId() == roomId)
            return rooms[i];
    }
    return nullptr;
}

int Home::getRoomCount() const { return roomCount; }
Room** Home::getRooms() const { return rooms; }

void Home::loadHomesFromFile(const std::string& filename, std::map<std::string, Home*>& allHomes) {
    for (auto& kv : allHomes)
        delete kv.second;
    allHomes.clear();
    std::ifstream fin(filename);
    if (!fin) {
        std::cerr << "Error: Cannot open " << filename << std::endl;
        return;
    }
    std::string line, homeId, homeName, ownerId;
    while (std::getline(fin, line)) {
        line = trim(line);
        if (line.empty()) {
            if (!homeId.empty() && !homeName.empty() && !ownerId.empty()) {
                Home* home = new Home(homeId, homeName, ownerId);
                allHomes[homeId] = home;
            }
            homeId.clear(); homeName.clear(); ownerId.clear();
            continue;
        }
        if (line.rfind("homeId:", 0) == 0) homeId = trim(line.substr(7));
        else if (line.rfind("homeName:", 0) == 0) homeName = trim(line.substr(9));
        else if (line.rfind("ownerId:", 0) == 0) ownerId = trim(line.substr(8));
    }
    if (!homeId.empty() && !homeName.empty() && !ownerId.empty()) {
        Home* home = new Home(homeId, homeName, ownerId);
        allHomes[homeId] = home;
    }
}

void Home::clearRooms() {
    for (int i = 0; i < roomCount; ++i)
        rooms[i] = nullptr;
    roomCount = 0;
}