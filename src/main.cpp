#include <iostream>
#include <pqxx/pqxx> // PostgreSQL C++ library
#include "User.h"
#include "UserDao.h"

int main() {
    // Hard-coded database connection parameters
    std::string dbname = "mydb";
    std::string dbuser = "postgres";
    std::string dbpassword = "postgres";
    std::string dbhost = "127.0.0.1";
    std::string dbport = "5432";

    // Create connection string
    std::string conn_str = "dbname=" + dbname + " user=" + dbuser + " password=" + dbpassword +
                           " hostaddr=" + dbhost + " port=" + dbport;

    try {
        pqxx::connection C(conn_str);
        if (C.is_open()) {
            std::cout << "Opened database successfully: " << C.dbname() << std::endl;

            UserDao userDao(C);

            // Create a new user
            User newUser(0, "john_doe", "password123"); // ID is not needed here
            if (userDao.createUser(newUser)) {
                std::cout << "User created successfully." << std::endl;
            }

            // Retrieve the user by ID
            User retrievedUser = userDao.getUserById(1);
            std::cout << "Retrieved User: " << retrievedUser.getUsername() << std::endl;

            // Update the user
            retrievedUser.setPassword("newpassword456");
            if (userDao.updateUser(retrievedUser)) {
                std::cout << "User updated successfully." << std::endl;
            }

            // Delete the user
//            if (userDao.deleteUser(1)) {
//                std::cout << "User deleted successfully." << std::endl;
//            }

            C.disconnect();
        } else {
            std::cerr << "Can't open database" << std::endl;
            return 1;
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
