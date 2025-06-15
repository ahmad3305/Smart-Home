#include "../include/Inventory.h"

Inventory::Inventory() : device(nullptr), warehouse(nullptr), quantity(0) {}
Inventory::Inventory(Device* device, Warehouse* warehouse, int quantity)
    : device(device), warehouse(warehouse), quantity(quantity) {
}

Device* Inventory::getDevice() const { return device; }
Warehouse* Inventory::getWarehouse() const { return warehouse; }
int Inventory::getQuantity() const { return quantity; }
void Inventory::setQuantity(int qty) { quantity = qty; }
void Inventory::addStock(int qty) { quantity += qty; }
bool Inventory::removeStock(int qty) {
    if (qty > quantity) return false;
    quantity -= qty;
    return true;
}