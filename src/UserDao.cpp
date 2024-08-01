#include "UserDao.h"
#include <iostream>

// Constructor
UserDao::UserDao(pqxx::connection& conn) : conn(conn) {}

bool UserDao::createUser(const User& user) {
    try {
        pqxx::work W(conn);
        W.exec0("INSERT INTO users (username, password) VALUES (" +
                W.quote(user.getUsername()) + ", " +
                W.quote(user.getPassword()) + ")");
        W.commit();
        return true;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
}

User UserDao::getUserById(int id) {
    try {
        pqxx::nontransaction N(conn);
        pqxx::result R = N.exec("SELECT id, username, password FROM users WHERE id = " + N.quote(id));

        if (R.size() != 1) {
            throw std::runtime_error("User not found");
        }

        const auto& row = R[0];
        return User(row["id"].as<int>(), row["username"].as<std::string>(), row["password"].as<std::string>());
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        throw;
    }
}

bool UserDao::updateUser(const User& user) {
    try {
        pqxx::work W(conn);
        W.exec0("UPDATE users SET username = " + W.quote(user.getUsername()) +
                ", password = " + W.quote(user.getPassword()) +
                " WHERE id = " + W.quote(user.getId()));
        W.commit();
        return true;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
}

bool UserDao::deleteUser(int id) {
    try {
        pqxx::work W(conn);
        W.exec0("DELETE FROM users WHERE id = " + W.quote(id));
        W.commit();
        return true;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
}
