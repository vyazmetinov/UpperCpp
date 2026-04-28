#pragma once
#include "Group.h"

class Users:public Groups {
public:
    std::map<int, User*> users;
    int createUser(int id, std::string name, int gid);

    int addUserToGroup(User *user, int gid);

    void allUsers();
    User* getUser(int id);
    void deleteUser(int id);
};