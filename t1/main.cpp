#include <iostream>
#include <map>
#include <cstring>
#include "Users.h"



void is_cmd(int argc, char *cmd[], Users &users, Groups &groups) {
    if (strcmp(cmd[0], "createUser") == 0) {
        int uid;
        char name[1024];
        int gid;
        std::sscanf(cmd[1], "{%d}", &uid);
        std::sscanf(cmd[2], "{%[^}]}", name);
        if (argc >= 4) {
            std::sscanf(cmd[3], "{%d}", &gid);
            users.createUser(uid, name, gid);
        }
        else {
            std::cerr << "Usage: createUser {uid} {name} {gid}" << std::endl;
        }

    }
    else if (strcmp(cmd[0], "createGroup") == 0) {
        if (argc >= 2) {
            int gid;
            std::sscanf(cmd[1], "{%d}", &gid);
            users.createGroup(gid);
        }
        else {
            std::cerr << "Usage: createGroup {gid}" << std::endl;
        }
    }
    else if (strcmp(cmd[0], "deleteGroup") == 0) {
        if (argc >= 2) {
            int gid;
            std::sscanf(cmd[1], "{%d}", &gid);
            users.deleteGroup(gid);
        }
        else {
            std::cerr << "Usage: deleteGroup {gid}" << std::endl;
        }
    }
    else if (strcmp(cmd[0], "allUsers") == 0) {
        users.allUsers();
    }
    else if (strcmp(cmd[0], "deleteUser") == 0) {
        if (argc >= 2) {
            int uid;
            std::sscanf(cmd[1], "{%d}", &uid);
            users.deleteUser(uid);
        }
        else {
            std::cerr << "Usage: deleteGroup {gid}" << std::endl;
        }
    }
    else if (strcmp(cmd[0], "allGroups") == 0) {
        users.allGroups();
    }
    else if (strcmp(cmd[0], "getGroup") == 0) {
        int gid;
        std::sscanf(cmd[1], "{%d}", &gid);
        std::cout << gid << std::endl;
        Group *gr = users.getGroup(gid);
        for (auto i : gr->members) {
            std::cout << i.second->id << " " << i.second->name << " " << i.second->group << std::endl;
        }

    }
    else if (strcmp(cmd[0], "getUser") == 0) {
        int uid;
        std::sscanf(cmd[1], "{%d}", &uid);
        std::cout << uid << std::endl;
        User *u = users.getUser(uid);
        std::cout << u->id << " " << u->name << " " << u->group << std::endl;
    }
}

int main() {
    Users users;
    Groups groups;
    std::string line;
    
    std::cout << "Введите команду (exit для выхода):" << std::endl;
    
    while (true) {
        std::cout << "> ";
        
        if (!std::getline(std::cin, line)) {
            break;
        }
        
        if (line.empty()) {
            continue;
        }
        
        if (line == "exit" || line == "quit") {
            break;
        }
        
        char* tokens[10];
        int token_count = 0;
        
        char* line_cstr = new char[line.length() + 1];
        std::strcpy(line_cstr, line.c_str());
        
        char* token = std::strtok(line_cstr, " ");
        while (token != nullptr && token_count < 10) {
            tokens[token_count++] = token;
            token = std::strtok(nullptr, " ");
        }
        
        if (token_count > 0) {
            is_cmd(token_count, tokens, users, groups);
        }
        
        delete[] line_cstr;
    }
    
    std::cout << "Программа завершена." << std::endl;
    return 0;
}