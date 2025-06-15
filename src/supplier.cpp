#include "../include/Supplier.h"

Supplier::Supplier() : supplierId(""), supplierName(""), contactInfo("") {}
Supplier::Supplier(const Mystr& id, const Mystr& name, const Mystr& contactInfo)
    : supplierId(id), supplierName(name), contactInfo(contactInfo) {
}

Mystr Supplier::getSupplierId() const { return supplierId; }
Mystr Supplier::getSupplierName() const { return supplierName; }
Mystr Supplier::getContactInfo() const { return contactInfo; }
void Supplier::setSupplierName(const Mystr& name) { supplierName = name; }
void Supplier::setContactInfo(const Mystr& info) { contactInfo = info; }