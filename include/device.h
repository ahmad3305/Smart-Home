#ifndef DEVICE_H
#define DEVICE_H

#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>

class Device;
class Room;

extern std::map<std::string, Device*> allDevices;

class Device {
protected:
    std::string deviceId;
    std::string deviceName;
    std::string deviceType;
    std::string roomId;
    bool status;
    double power;

public:
    Device();
    Device(const std::string& id,
           const std::string& name,
           const std::string& type,
           const std::string& room,
           bool status,
           double power);

    std::string getDeviceId() const;
    std::string getDeviceName() const;
    std::string getDeviceType() const;
    std::string getRoomId() const;
    bool getStatus() const;
    double getPower() const;

    void setDeviceId(const std::string& id);
    void setDeviceName(const std::string& name);
    void setDeviceType(const std::string& type);
    void setRoomId(const std::string& room);
    void setStatus(bool st);
    void setPower(double p);

    virtual void updateDeviceFile(const std::string& devicesFile) const = 0;
    void updateMainDeviceFile(const std::string& devicesFile) const;

    static void loadDevicesFromFile(const std::string& filename, std::map<std::string, Room*>& allRooms);
    virtual void addToFile() {}
    virtual ~Device() {}
};

#endif