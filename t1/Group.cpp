#include <iostream>
#include <ostream>

#include "Users.h"

Group::Group() {
    
}

int Users::addUserToGroup(User *user, int gid) {
    if (groups.find(gid) == groups.end()) {
        std::cerr << "Group does not exist" << std::endl;
        return 0;
    }
    
    if (groups[gid]->members.find(user->id) == groups[gid]->members.end()) {
        groups[gid]->members[user->id] = user;
        users[user->id]->group = groups[gid];
    }
    else {
        std::cerr << "User already exists in group" << std::endl;
        return 0;
    }
    return 1;
}

Group* Groups::getGroup(int gid) {
    std::cout << gid << std::endl;
    if (groups.find(gid) == groups.end()) {
        std::cerr << "Group not found\n";
    }
    else {
        return groups[gid];
    }
    return nullptr;
}

Groups::Groups(){

}

void Groups::createGroup(int gid) {
    if (groups.find(gid) == groups.end()) {
        groups[gid] = new Group();
        std::cout << "Group " << gid << " created" << std::endl;
    }
    else {
        std::cerr << "Group already exists" << std::endl;
    }
}

void Groups::deleteGroup(int gid) {
    if (groups.find(gid) != groups.end()) {
        delete groups[gid];
        groups.erase(gid);
        std::cout << "Group " << gid << " deleted" << std::endl;
    }
    else {
        std::cerr << "Group does not exist" << std::endl;
    }
}

void Users::allUsers(){
    for (auto i : users) {
        std::cout << i.second->id << " " << i.second->name << " " << i.second->group << std::endl;
    }
}

void Users::deleteUser(int id) {
    if (users.find(id) != users.end()) {
        delete users[id];
        users.erase(id);
    }
    else {
        std::cerr << "User not found" << std::endl;
    }
}

void Groups::allGroups() {
    for (auto i : groups) {
        std::cout << i.first << '\n';
        for (auto j : i.second->members) {
            std::cout  << j.second->id << " " << j.second->name << " " << j.second->group << std::endl;
        }
    }
}

User* Users::getUser(int id) {
    if (users.find(id) != users.end()) {
        return users[id];
    }
    std::cerr << "User not found" << std::endl;
    return nullptr;
}





