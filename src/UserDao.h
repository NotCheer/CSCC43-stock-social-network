//
// Created by xinyuli on 8/1/24.
//

#ifndef CSCC43_STOCK_SOCIAL_NETWORK_USERDAO_H
#define CSCC43_STOCK_SOCIAL_NETWORK_USERDAO_H


#include <pqxx/pqxx>
#include "User.h"

class UserDao {
public:
    UserDao(pqxx::connection& conn);

    bool createUser(const User& user);
    User getUserById(int id);
    bool updateUser(const User& user);
    bool deleteUser(int id);

private:
    pqxx::connection& conn;
};

#endif //CSCC43_STOCK_SOCIAL_NETWORK_USERDAO_H
