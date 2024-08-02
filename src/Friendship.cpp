#include "Friendship.h"

Friendship::Friendship(int user_id, int friend_id, const std::string& status, std::time_t request_date, std::time_t response_date)
        : user_id(user_id), friend_id(friend_id), status(status), request_date(request_date), response_date(response_date) {}

Friendship::Friendship(int friendship_id, int user_id, int friend_id, const std::string& status, std::time_t request_date, std::time_t response_date)
        : friendship_id(friendship_id), user_id(user_id), friend_id(friend_id), status(status), request_date(request_date), response_date(response_date) {}

int Friendship::getFriendshipId() const {
    return friendship_id;
}

int Friendship::getUserId() const {
    return user_id;
}

int Friendship::getFriendId() const {
    return friend_id;
}

std::string Friendship::getStatus() const {
    return status;
}

std::time_t Friendship::getRequestDate() const {
    return request_date;
}

std::time_t Friendship::getResponseDate() const {
    return response_date;
}

void Friendship::setUserId(int user_id) {
    this->user_id = user_id;
}

void Friendship::setFriendId(int friend_id) {
    this->friend_id = friend_id;
}

void Friendship::setStatus(const std::string& status) {
    this->status = status;
}

void Friendship::setRequestDate(std::time_t request_date) {
    this->request_date = request_date;
}

void Friendship::setResponseDate(std::time_t response_date) {
    this->response_date = response_date;
}
