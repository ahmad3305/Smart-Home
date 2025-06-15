#ifndef ROOM_H
#define ROOM_H

#include <string>
#include <vector>
#include <map>

class Device;

class Room {
    std::string roomId;
    std::string roomName;
    std::string homeId;
    Device** devices;
    int deviceCount;
    int deviceCapacity;
public:
    Room();
    Room(const std::string& roomId, const std::string& roomName, const std::string& homeId);
    ~Room();

    std::string getRoomId() const;
    std::string getRoomName() const;
    std::string getHomeId() const;
    void setRoomName(const std::string& newName);

    void addDevice(Device* device);
    void removeDevice(const std::string& deviceId);
    Device* getDevice(const std::string& deviceId) const;
    int getDeviceCount() const;
    Device** getDevices() const;
    std::vector<Device*> getDevicesVector() const;

    void setHomeId(const std::string& newHomeId);
    void clearDevices();
    static Room* addRoomToHome(const std::string& roomId, const std::string& roomName, const std::string& homeId);

    static void loadRoomsFromFile(const std::string& filename, std::map<std::string, Room*>& allRooms, std::map<std::string, class Home*>& allHomes);

};

#endif // ROOM_H