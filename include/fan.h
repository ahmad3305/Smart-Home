#ifndef FAN_H
#define FAN_H

#include "device.h"
#include <string>
#include <map>
#include <fstream>
#include <iostream>

inline std::string trim_bothh(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

class Fan : public Device {
protected:
    int speed = 0;
public:
    Fan() : Device(), speed(0) {}

    Fan(const std::string& id, const std::string& name, const std::string& roomId, bool status, double power)
        : Device(id, name, "Fan", roomId, status, power), speed(0) {
    }

    Fan(const std::string& id, const std::string& name)
        : Device(id, name, "Fan", "", false, 0.0), speed(0) {
    }

    int getSpeed() const;
    void setSpeed(int s);

    static std::map<std::string, Fan*> loadFanFeaturesFromFile(const std::string& filename);
    void updateDeviceFile(const std::string& devicesfile = "data/fan.txt") const override;

    virtual ~Fan() {}

    void addToFile() override;
};

#endif // FAN_H