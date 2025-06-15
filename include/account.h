#ifndef ACCOUNT_H
#define ACCOUNT_H
#include <string>
using namespace std;

class Account {
protected:
    string id;
    string username;
    string password;
    string name;
public:
    Account();
    Account(const string& id, const string& username, const string& password, const string& name);
    virtual ~Account();

    string getId() const;
    string getUsername() const;
    string getName() const;
    void setPassword(const string& newPassword);
    virtual string getRole() const = 0;
};

#endif
