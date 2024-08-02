#include <iostream>
#include "../src/FriendshipDao.h"

int main() {
    std::string dbname = "mydb";
    std::string dbuser = "postgres";
    std::string dbpassword = "postgres";
    std::string dbhost = "127.0.0.1";
    std::string dbport = "5432";

    // Create connection string
    std::string conn_str = "dbname=" + dbname + " user=" + dbuser + " password=" + dbpassword +
                           " hostaddr=" + dbhost + " port=" + dbport;

    try {
        FriendshipDao friendshipDao(conn_str);

        std::cout << "Connected to the database successfully!" << std::endl;

        // Create a new friend request
        int newFriendRequestId = friendshipDao.createFriendRequest(1, 2);
        std::cout << "Created Friend Request with ID: " << newFriendRequestId << std::endl;

        // Accept the friend request
        friendshipDao.acceptFriendRequest(newFriendRequestId);
        std::cout << "Accepted Friend Request with ID: " << newFriendRequestId << std::endl;

        // Retrieve the accepted friendship
        Friendship acceptedFriendship = friendshipDao.getFriendshipById(newFriendRequestId);
        std::cout << "Retrieved Friendship between User " << acceptedFriendship.getUserId() << " and User " << acceptedFriendship.getFriendId() << " with status " << acceptedFriendship.getStatus() << std::endl;

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
