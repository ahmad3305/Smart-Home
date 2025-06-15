#ifndef LIGHT_H
#define LIGHT_H

#include "device.h"
#include <string>
#include <map>
#include <fstream>
#include <iostream>

class Light : public Device {
protected:
    int brightness = 0;
public:
    Light();

    Light(const std::string& id, const std::string& name, const std::string& roomId, bool status, double power)
        : Device(id, name, "Light", roomId, status, power), brightness(0) {
    }

    Light(const std::string& id, const std::string& name)
        : Device(id, name, "Light", "", false, 0.0), brightness(0) {
    }

    int getBrightness() const;
    void setBrightness(int b);

    void setDeviceName(const std::string& name) { Device::setDeviceName(name); }
    std::string getDeviceName() const { return Device::getDeviceName(); }

    static std::map<std::string, Light*> loadLightFeaturesFromFile(const std::string& filename);

    void updateDeviceFile(const std::string& ldevicesfile = "data/light.txt") const override;

    void addToFile() override;
    virtual ~Light() {}
};

#endif