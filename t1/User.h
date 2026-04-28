#pragma once
#include <string>
#include <map>

class Group;

class User {
    public:
    std::string name;
    int id;
    Group *group;
    User(std::string name, int id, Group* group) : name(name), id(id), group(group) {}
    User(std::string name, int id) : name(name), id(id), group(nullptr) {}
    User() : name(""), id(-1), group(nullptr) {}
};


