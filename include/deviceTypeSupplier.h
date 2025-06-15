#ifndef DEVICETYPESUPPLIER_H
#define DEVICETYPESUPPLIER_H

#include "DeviceType.h"
#include "Supplier.h"
using namespace std;

class DeviceTypeSupplier {
    DeviceType deviceType;
    Supplier* supplier;
    double price;
    int warrantyMonths;
public:
    DeviceTypeSupplier();
    DeviceTypeSupplier(const DeviceType& dt, Supplier* s, double price, int warrantyMonths);

    DeviceType getDeviceType() const;
    Supplier* getSupplier() const;
    double getPrice() const;
    int getWarrantyMonths() const;

    void setPrice(double price);
    void setWarrantyMonths(int months);
};

#endif