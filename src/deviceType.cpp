#include "../include/deviceType.h"

DeviceType::DeviceType() : typeName("") {}
DeviceType::DeviceType(const Mystr& typeName) : typeName(typeName) {}
Mystr DeviceType::getTypeName() const { return typeName; }
void DeviceType::setTypeName(const Mystr& newType) { typeName = newType; }