#ifndef USER_H
#define USER_H

#include "account.h"
#include <string>
#include <map>

class User;
extern std::map<std::string, User*> allUsers;

class User : public Account {
public:
    User();
    User(const std::string& id, const std::string& username, const std::string& password, const std::string& name);
    std::string getRole() const override;

    const std::string& getUserId() const { return id; }
    const std::string& getUsernameValue() const { return username; }
    const std::string& getNameValue() const { return name; }

    bool validatePassword(const std::string& inputPassword) const;


    static void loadUsersFromFile(const std::string& filename, std::map<std::string, User*>& allUsers);
};

#endif