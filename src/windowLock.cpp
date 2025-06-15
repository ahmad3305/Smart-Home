#include "../include/windowlock.h"
#include <fstream>
#include <vector>
#include <string>


WindowLock::WindowLock() : Device() {}

bool WindowLock::isLocked() const {
    return getStatus();
}

void WindowLock::setLocked(bool l) {
    setStatus(l);
}
