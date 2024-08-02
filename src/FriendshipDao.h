//
// Created by xinyuli on 8/2/24.
//

#ifndef CSCC43_STOCK_SOCIAL_NETWORK_FRIENDSHIPDAO_H
#define CSCC43_STOCK_SOCIAL_NETWORK_FRIENDSHIPDAO_H


#include <string>
#include <vector>
#include <pqxx/pqxx>
#include "Friendship.h"

class FriendshipDao {
private:
    std::string connection_string;

public:
    FriendshipDao(const std::string& conn_str);

    int addFriendship(const Friendship& friendship);
    Friendship getFriendshipById(int friendship_id);
    std::vector<Friendship> getFriendshipsByUserId(int user_id);
    void deleteFriendship(int friendship_id);

    int createFriendRequest(int user_id, int friend_id);
    void acceptFriendRequest(int friendship_id);
};


#endif //CSCC43_STOCK_SOCIAL_NETWORK_FRIENDSHIPDAO_H
