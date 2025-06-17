#include "../include/device.h"
#include "../include/room.h"
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>
#include "../include/Light.h"
#include "../include/Fan.h"
#include "../include/AC.h"
#include "../include/Heater.h"
#include "../include/Wifi.h"
#include "../include/WindowLock.h"
#include "../include/DoorLock.h"


static std::string trim_bothhhh(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) return "";
    size_t end = s.find_last_not_of(" \t\n\r");
    return s.substr(start, end - start + 1);
}

Device::Device()
    : deviceId(""), deviceName(""), deviceType(""), roomId(""), status(false), power(0.0) {
}

Device::Device(const std::string& id,
    const std::string& name,
    const std::string& type,
    const std::string& room,
    bool st,
    double p)
    : deviceId(id), deviceName(name), deviceType(type), roomId(room), status(st), power(p) {
}

std::string Device::getDeviceId() const { return deviceId; }
std::string Device::getDeviceName() const { return deviceName; }
std::string Device::getDeviceType() const { return deviceType; }
std::string Device::getRoomId() const { return roomId; }
bool Device::getStatus() const { return status; }
double Device::getPower() const { return power; }

void Device::setDeviceId(const std::string& id) { deviceId = id; }
void Device::setDeviceName(const std::string& name) { deviceName = name; }
void Device::setDeviceType(const std::string& type) { deviceType = type; }
void Device::setRoomId(const std::string& room) { roomId = room; }
void Device::setStatus(bool st) { status = st; }
void Device::setPower(double p) { power = p; }

void Device::updateMainDeviceFile(const std::string& devicesFile) const {
    std::ifstream infile(devicesFile);
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(infile, line)) lines.push_back(line);
    infile.close();

    std::ofstream outfile(devicesFile);
    bool found = false;
    for (size_t i = 0; i < lines.size(); ++i) {
        std::string trimmed = trim_bothhhh(lines[i]);
        if (trimmed.rfind("deviceId:", 0) == 0) {
            std::string fileId = trim_bothhhh(trimmed.substr(9));
            if (fileId == getDeviceId()) {
                found = true;
                outfile << lines[i] << "\n";
                size_t j = i + 1;
                bool statusWritten = false;
                while (j < lines.size() && !trim_bothhhh(lines[j]).empty()) {
                    if (trim_bothhhh(lines[j]).rfind("status:", 0) == 0) {
                        outfile << "status: " << (getStatus() ? "1" : "0") << "\n";
                        statusWritten = true;
                    }
                    else {
                        outfile << lines[j] << "\n";
                    }
                    ++j;
                }
                if (!statusWritten)
                    outfile << "status: " << (getStatus() ? "1" : "0") << "\n";
                outfile << "\n";
                i = j;
                continue;
            }
        }
        outfile << lines[i] << "\n";
    }
    outfile.close();
    if (!found) std::cerr << "Device ID " << getDeviceId() << " not found in devices.txt\n";
}

void Device::loadDevicesFromFile(const std::string& filename, std::map<std::string, Room*>& allRooms) {

    for (auto& kv : allRooms)
        if (kv.second)
            kv.second->clearDevices();

    std::ifstream fin(filename);
    if (!fin) {
        std::cerr << "Error: Cannot open " << filename << std::endl;
        return;
    }

    std::string line, deviceId, deviceName, deviceType, roomId, statusStr, powerStr;
    bool status = false;
    double power = 0.0;

    auto createAndAddDevice = [&]() {
        if (deviceId.empty() || deviceName.empty() || deviceType.empty() || roomId.empty())
            return;
        Device* dev = nullptr;
        if (deviceType == "Light")
            dev = new Light(deviceId, deviceName, roomId, status, power);
        else if (deviceType == "Fan")
            dev = new Fan(deviceId, deviceName, roomId, status, power);
        else if (deviceType == "AC")
            dev = new AC(deviceId, deviceName, roomId, status, power);
        else if (deviceType == "Heater")
            dev = new Heater(deviceId, deviceName, roomId, status, power);
        else if (deviceType == "Wifi")
            dev = new Wifi(deviceId, deviceName, roomId, status, power);
        else if (deviceType == "WindowLock")
            dev = new WindowLock(deviceId, deviceName, roomId, status, power);
        else if (deviceType == "DoorLock")
            dev = new DoorLock(deviceId, deviceName, roomId, status, power);

        allDevices[deviceId] = dev;

        auto it = allRooms.find(roomId);
        if (it != allRooms.end() && it->second) {
            it->second->addDevice(dev);
        }
        else {
            allDevices.erase(deviceId);
            delete dev;
        }

        deviceId.clear();
        deviceName.clear();
        deviceType.clear();
        roomId.clear();
        statusStr.clear();
        powerStr.clear();
        status = false;
        power = 0.0;
        };

    while (std::getline(fin, line)) {
        line = trim_bothhhh(line);

        if (line.empty()) continue;

        if (line.rfind("deviceId:", 0) == 0) {
            createAndAddDevice();
            deviceId = trim_bothhhh(line.substr(9));
        }
        else if (line.rfind("deviceName:", 0) == 0)
            deviceName = trim_bothhhh(line.substr(11));
        else if (line.rfind("deviceType:", 0) == 0)
            deviceType = trim_bothhhh(line.substr(11));
        else if (line.rfind("roomId:", 0) == 0)
            roomId = trim_bothhhh(line.substr(7));
        else if (line.rfind("status:", 0) == 0) {
            statusStr = trim_bothhhh(line.substr(7));
            status = (statusStr == "1");
        }
        else if (line.rfind("power:", 0) == 0) {
            powerStr = trim_bothhhh(line.substr(6));
            power = powerStr.empty() ? 0.0 : std::stod(powerStr);
        }
    }
    createAndAddDevice();
}