#include "../include/account.h"
Account::Account() {}

Account::Account(const string& id, const string& username, const string& password, const string& name)
    : id(id), username(username), password(password), name(name) {
}

Account::~Account() {}

string Account::getId() const {
    return id;
}

string Account::getUsername() const {
    return username;
}

string Account::getName() const {
    return name;
}

void Account::setPassword(const string& newPassword) {
    password = newPassword;
}

string Account::getRole() const {
    return string("account");
}