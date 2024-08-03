//
// Created by xinyuli on 8/1/24.
//

#ifndef CSCC43_STOCK_SOCIAL_NETWORK_USER_H
#define CSCC43_STOCK_SOCIAL_NETWORK_USER_H


#include <string>

class User {
public:
    // Constructors
    User() = default;
    User(int id, const std::string& username, const std::string& password);

    // Getters
    int getId() const;
    std::string getUsername() const;
    std::string getPassword() const;

    // Setters
    void setUsername(const std::string& username);
    void setPassword(const std::string& password);

private:
    int id;
    std::string username;
    std::string password;
};


#endif //CSCC43_STOCK_SOCIAL_NETWORK_USER_H
