#ifndef AC_H
#define AC_H

#include "device.h"
#include <string>
#include <map>
#include <fstream>
#include <iostream>

class AC : public Device {
protected:
    int temperature = 0;
public:
    AC();
    AC(const std::string& id, const std::string& name, const std::string& roomId, bool status, double power)
        : Device(id, name, "AC", roomId, status, power), temperature(0) {}
    AC(const std::string& id, const std::string& name)
        : Device(id, name, "AC", "", false, 0.0), temperature(0) {}

    int getTemperature() const { return temperature; }
    void setTemperature(int t);

    void setDeviceName(const std::string& name) { Device::setDeviceName(name); }
    std::string getDeviceName() const { return Device::getDeviceName(); }

    static std::map<std::string, AC*> loadACFeaturesFromFile(const std::string& filename);

    void updateDeviceFile(const std::string& devicesFile = "data/ac.txt") const override;
    void addToFile() override;
    virtual ~AC() {}
};

#endif // AC_H