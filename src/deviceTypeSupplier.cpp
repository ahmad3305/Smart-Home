#include "../include/DeviceTypeSupplier.h"

DeviceTypeSupplier::DeviceTypeSupplier()
    : deviceType(), supplier(nullptr), price(0.0), warrantyMonths(0) {
}

DeviceTypeSupplier::DeviceTypeSupplier(const DeviceType& dt, Supplier* s, double price, int warrantyMonths)
    : deviceType(dt), supplier(s), price(price), warrantyMonths(warrantyMonths) {
}

DeviceType DeviceTypeSupplier::getDeviceType() const { return deviceType; }
Supplier* DeviceTypeSupplier::getSupplier() const { return supplier; }
double DeviceTypeSupplier::getPrice() const { return price; }
int DeviceTypeSupplier::getWarrantyMonths() const { return warrantyMonths; }

void DeviceTypeSupplier::setPrice(double p) { price = p; }
void DeviceTypeSupplier::setWarrantyMonths(int m) { warrantyMonths = m; }