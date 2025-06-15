#ifndef WIFI_H
#define WIFI_H

#include "device.h"
#include <string>
#include <map>
#include <fstream>
#include <iostream>

class Wifi : public Device {
protected:
    std::string ssid;
public:
    Wifi();
    Wifi(const std::string& id, const std::string& name, const std::string& roomId, bool status, double power)
        : Device(id, name, "Wifi", roomId, status, power), ssid(name) {
    }
    Wifi(const std::string& id, const std::string& name) {
        setDeviceId(id);
        setDeviceName(name);
        ssid = "";
    }

    std::string getSSID() const;
    void setSSID(const std::string& s);

    std::string getDeviceName() const { return Device::getDeviceName(); }

    void updateDeviceFile(const std::string& wdevicesfile = "data/wifi.txt") const override;

    static std::map<std::string, Wifi*> loadWifiFeaturesFromFile(const std::string& filename);
    void addToFile() override;

    virtual ~Wifi() {}
};

#endif