#include "Users.h"
#include <iostream>
#include <ostream>

int Users::createUser(int id, std::string name, int gid) {
    if (users.find(id) == users.end()) {
        users[id] = new User(name, id);
        std::cout << "User " << id << " (" << name << ") created" << std::endl;
        if (addUserToGroup(users[id], gid)) {
            std::cout << "User " << id << " added to group " << gid << std::endl;
        }
    }
    else {
        std::cout << "User already exists" << std::endl;
    }
    return 0;
}
