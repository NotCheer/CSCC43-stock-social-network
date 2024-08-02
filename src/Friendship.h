//
// Created by xinyuli on 8/2/24.
//

#ifndef CSCC43_STOCK_SOCIAL_NETWORK_FRIENDSHIP_H
#define CSCC43_STOCK_SOCIAL_NETWORK_FRIENDSHIP_H


#include <ctime>
#include <string>

class Friendship {
private:
    int friendship_id;
    int user_id;
    int friend_id;
    std::string status;
    std::time_t request_date;
    std::time_t response_date;

public:
    Friendship(int user_id, int friend_id, const std::string& status, std::time_t request_date, std::time_t response_date);
    Friendship(int friendship_id, int user_id, int friend_id, const std::string& status, std::time_t request_date, std::time_t response_date);

    int getFriendshipId() const;
    int getUserId() const;
    int getFriendId() const;
    std::string getStatus() const;
    std::time_t getRequestDate() const;
    std::time_t getResponseDate() const;

    void setUserId(int user_id);
    void setFriendId(int friend_id);
    void setStatus(const std::string& status);
    void setRequestDate(std::time_t request_date);
    void setResponseDate(std::time_t response_date);
};




#endif //CSCC43_STOCK_SOCIAL_NETWORK_FRIENDSHIP_H
