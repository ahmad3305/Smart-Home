#ifndef ADMIN_H
#define ADMIN_H
#include "account.h"
using namespace std;

class Admin : public Account {
public:
    Admin();
    Admin(const string& id, const string& username, const string& password, const string& name);
};

#endif
