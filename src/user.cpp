#include "../include/User.h"
#include <fstream>
#include <sstream>

User::User() : Account() {}

User::User(const std::string& id, const std::string& username, const std::string& password, const std::string& name)
    : Account(id, username, password, name) {
}

bool User::validatePassword(const std::string& inputPassword) const {
    return password == inputPassword;
}

std::string User::getRole() const {
    return "user";
}

void User::loadUsersFromFile(const std::string& filename, std::map<std::string, User*>& allUsers) {
    std::ifstream file(filename);
    std::string line, userId, username, password, name;
    while (getline(file, line)) {
        line.erase(line.find_last_not_of(" \n\r\t") + 1);

        if (line.rfind("userId:", 0) == 0) {
            userId = line.substr(8);
        }
        else if (line.rfind("username:", 0) == 0) {
            username = line.substr(10);
        }
        else if (line.rfind("password:", 0) == 0) {
            password = line.substr(10);
        }
        else if (line.rfind("name:", 0) == 0) {
            name = line.substr(6);
        }
        else if (line.empty() && !userId.empty()) {
            allUsers[userId] = new User(userId, username, password, name);
            userId.clear(); username.clear(); password.clear(); name.clear();
        }
    }
    if (!userId.empty()) {
        allUsers[userId] = new User(userId, username, password, name);
    }
}