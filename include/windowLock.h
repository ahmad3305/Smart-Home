#ifndef WINDOWLOCK_H
#define WINDOWLOCK_H

#include "device.h"
#include <string>

class WindowLock : public Device {
public:
    WindowLock();

    WindowLock(const std::string& id, const std::string& name, const std::string& roomId, bool status, double power)
        : Device(id, name, "WindowLock", roomId, status, power) {
    }

    WindowLock(const std::string& id, const std::string& name)
        : Device(id, name, "WindowLock", "", false, 0.0) {
    }

    bool isLocked() const;
    void setLocked(bool l);

    void setDeviceName(const std::string& name) { Device::setDeviceName(name); }
    std::string getDeviceName() const { return Device::getDeviceName(); }

    virtual void updateDeviceFile(const std::string& devicesFile) const override {}


    virtual ~WindowLock() {}
};

#endif