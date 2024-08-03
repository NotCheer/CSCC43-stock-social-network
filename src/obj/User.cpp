#include "User.h"

// Constructor
User::User(int id, const std::string& username, const std::string& password)
        : id(id), username(username), password(password) {}

// Getters
int User::getId() const {
    return id;
}

std::string User::getUsername() const {
    return username;
}

std::string User::getPassword() const {
    return password;
}

// Setters
void User::setUsername(const std::string& username) {
    this->username = username;
}

void User::setPassword(const std::string& password) {
    this->password = password;
}
