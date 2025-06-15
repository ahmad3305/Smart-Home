#ifndef DEVICETYPEWORKER_H
#define DEVICETYPEWORKER_H

#include "DeviceType.h"
#include "Worker.h"
using namespace std;

class DeviceTypeWorker {
    DeviceType deviceType;
    Worker* worker;
    Mystr skillLevel;
public:
    DeviceTypeWorker();
    DeviceTypeWorker(const DeviceType& dt, Worker* w, const Mystr& skillLevel);

    DeviceType getDeviceType() const;
    Worker* getWorker() const;
    Mystr getSkillLevel() const;

    void setSkillLevel(const Mystr& level);
};

#endif