#ifndef SUPPLIER_H
#define SUPPLIER_H

#include "mystr.h"
using namespace std;

class Supplier {
    Mystr supplierId;
    Mystr supplierName;
    Mystr contactInfo;
public:
    Supplier();
    Supplier(const Mystr& id, const Mystr& name, const Mystr& contactInfo);

    Mystr getSupplierId() const;
    Mystr getSupplierName() const;
    Mystr getContactInfo() const;
    void setSupplierName(const Mystr& name);
    void setContactInfo(const Mystr& info);
};

#endif