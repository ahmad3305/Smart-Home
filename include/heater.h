#ifndef HEATER_H
#define HEATER_H

#include "device.h"
#include <string>
#include <map>
#include <fstream>
#include <iostream>

class Heater : public Device {
protected:
    int temperature = 0;
public:
    Heater();
    Heater(const std::string& id, const std::string& name, const std::string& roomId, bool status, double power)
        : Device(id, name, "Heater", roomId, status, power), temperature(0) {
    }
    Heater(const std::string& id, const std::string& name)
        : Device(id, name, "Heater", "", false, 0.0), temperature(0) {
    }

    int getTemperature() const;
    void setTemperature(int t);

    void setDeviceName(const std::string& name) { Device::setDeviceName(name); }
    std::string getDeviceName() const { return Device::getDeviceName(); }

    static std::map<std::string, Heater*> loadHeaterFeaturesFromFile(const std::string& filename);

    void updateDeviceFile(const std::string& devicesfile = "data/heater.txt") const override;

    void addToFile() override;
    virtual ~Heater() {}
};

#endif