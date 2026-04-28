#pragma once
#include <map>

#include "User.h"

class Group:public User {
    public:
    std::map<int, User*> members;
    Group();
};

class Groups:public Group {
    public:
    std::map<int, Group*> groups;
    Groups();

    void createGroup(int gid);
    void deleteGroup(int gid);
    void allGroups();
    Group* getGroup(int gid);

};
