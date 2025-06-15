#include "../include/doorlock.h"
#include <fstream>
#include <vector>
#include <string>


DoorLock::DoorLock() : Device() {}

bool DoorLock::isLocked() const {
    return getStatus();
}

void DoorLock::setLocked(bool l) {
    setStatus(l);
}
