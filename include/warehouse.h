#ifndef WAREHOUSE_H
#define WAREHOUSE_H

#include "mystr.h"
using namespace std;

class Warehouse {
    Mystr warehouseId;
    Mystr location;
public:
    Warehouse();
    Warehouse(const Mystr& id, const Mystr& loc);

    Mystr getWarehouseId() const;
    Mystr getLocation() const;
    void setLocation(const Mystr& loc);
};

#endif