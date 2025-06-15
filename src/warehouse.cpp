#include "../include/Warehouse.h"

Warehouse::Warehouse() : warehouseId(""), location("") {}
Warehouse::Warehouse(const Mystr& id, const Mystr& loc)
    : warehouseId(id), location(loc) {
}

Mystr Warehouse::getWarehouseId() const { return warehouseId; }
Mystr Warehouse::getLocation() const { return location; }
void Warehouse::setLocation(const Mystr& loc) { location = loc; }