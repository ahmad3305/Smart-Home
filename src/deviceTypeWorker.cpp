#include "../include/DeviceTypeWorker.h"

DeviceTypeWorker::DeviceTypeWorker() : deviceType(), worker(nullptr), skillLevel("Beginner") {}

DeviceTypeWorker::DeviceTypeWorker(const DeviceType& dt, Worker* w, const Mystr& skillLevel)
    : deviceType(dt), worker(w), skillLevel(skillLevel) {
}

DeviceType DeviceTypeWorker::getDeviceType() const { return deviceType; }
Worker* DeviceTypeWorker::getWorker() const { return worker; }
Mystr DeviceTypeWorker::getSkillLevel() const { return skillLevel; }
void DeviceTypeWorker::setSkillLevel(const Mystr& level) { skillLevel = level; }