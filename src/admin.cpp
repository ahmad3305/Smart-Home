#include "../include/admin.h"

Admin::Admin() : Account() {}

Admin::Admin(const string& id, const string& username, const string& password, const string& name)
    : Account(id, username, password, name) {
}
