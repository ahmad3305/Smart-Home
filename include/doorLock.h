#ifndef DOORLOCK_H
#define DOORLOCK_H

#include "device.h"
#include <string>

class DoorLock : public Device {
public:
    DoorLock();
    DoorLock(const std::string& id, const std::string& name, const std::string& roomId, bool status, double power)
        : Device(id, name, "DoorLock", roomId, status, power) {
    }
    DoorLock(const std::string& id, const std::string& name)
        : Device(id, name, "DoorLock", "", false, 0.0) {
    }

    bool isLocked() const;
    void setLocked(bool l);
    
    virtual void updateDeviceFile(const std::string& devicesFile) const override {}


    virtual ~DoorLock() {}
};

#endif