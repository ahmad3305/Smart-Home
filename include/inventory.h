#ifndef INVENTORY_H
#define INVENTORY_H

#include "Device.h"
#include "Warehouse.h"
using namespace std;

class Inventory {
    Device* device;
    Warehouse* warehouse;
    int quantity;
public:
    Inventory();
    Inventory(Device* device, Warehouse* warehouse, int quantity);

    Device* getDevice() const;
    Warehouse* getWarehouse() const;
    int getQuantity() const;
    void setQuantity(int qty);
    void addStock(int qty);
    bool removeStock(int qty);
};

#endif