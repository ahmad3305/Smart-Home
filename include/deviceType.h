#ifndef DEVICETYPE_H
#define DEVICETYPE_H

#include "mystr.h"
using namespace std;

class DeviceType {
    Mystr typeName;
public:
    DeviceType();
    DeviceType(const Mystr& typeName);
    Mystr getTypeName() const;
    void setTypeName(const Mystr& newType);
};

#endif