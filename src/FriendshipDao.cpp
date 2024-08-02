#include "FriendshipDao.h"
#include <iostream>
#include <iomanip>
#include <sstream>

FriendshipDao::FriendshipDao(const std::string& conn_str) : connection_string(conn_str) {}

std::string formatTime(std::time_t time) {
    std::tm *tm = std::gmtime(&time);
    std::stringstream ss;
    ss << std::put_time(tm, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

int FriendshipDao::addFriendship(const Friendship& friendship) {
    try {
        pqxx::connection C(connection_string);
        pqxx::work W(C);
        std::string sql = "INSERT INTO friendships (user_id, friend_id, status, request_date, response_date) VALUES (" +
                          W.quote(friendship.getUserId()) + ", " +
                          W.quote(friendship.getFriendId()) + ", " +
                          W.quote(friendship.getStatus()) + ", " +
                          W.quote(formatTime(friendship.getRequestDate())) + ", " +
                          W.quote(formatTime(friendship.getResponseDate())) + ") RETURNING friendship_id";
        pqxx::result R = W.exec(sql);
        W.commit();
        return R[0][0].as<int>();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        throw;
    }
}

Friendship FriendshipDao::getFriendshipById(int friendship_id) {
    try {
        pqxx::connection C(connection_string);
        pqxx::nontransaction N(C);
        std::string sql = "SELECT friendship_id, user_id, friend_id, status, request_date, response_date FROM friendships WHERE friendship_id = " + N.quote(friendship_id);
        pqxx::result R(N.exec(sql));

        if (R.size() != 1) {
            throw std::runtime_error("Friendship not found");
        }

        pqxx::row row = R[0];
        std::tm tm_request = {};
        std::istringstream ss_request(row["request_date"].as<std::string>());
        ss_request >> std::get_time(&tm_request, "%Y-%m-%d %H:%M:%S");
        std::time_t request_date = std::mktime(&tm_request);

        std::tm tm_response = {};
        std::istringstream ss_response(row["response_date"].as<std::string>());
        ss_response >> std::get_time(&tm_response, "%Y-%m-%d %H:%M:%S");
        std::time_t response_date = std::mktime(&tm_response);

        return Friendship(row["friendship_id"].as<int>(), row["user_id"].as<int>(), row["friend_id"].as<int>(), row["status"].as<std::string>(), request_date, response_date);
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        throw;
    }
}

std::vector<Friendship> FriendshipDao::getFriendshipsByUserId(int user_id) {
    std::vector<Friendship> friendships;
    try {
        pqxx::connection C(connection_string);
        pqxx::nontransaction N(C);
        std::string sql = "SELECT friendship_id, user_id, friend_id, status, request_date, response_date FROM friendships WHERE user_id = " + N.quote(user_id) +
                          " OR friend_id = " + N.quote(user_id);
        pqxx::result R(N.exec(sql));

        for (auto row : R) {
            std::tm tm_request = {};
            std::istringstream ss_request(row["request_date"].as<std::string>());
            ss_request >> std::get_time(&tm_request, "%Y-%m-%d %H:%M:%S");
            std::time_t request_date = std::mktime(&tm_request);

            std::tm tm_response = {};
            std::istringstream ss_response(row["response_date"].as<std::string>());
            ss_response >> std::get_time(&tm_response, "%Y-%m-%d %H:%M:%S");
            std::time_t response_date = std::mktime(&tm_response);

            friendships.emplace_back(row["friendship_id"].as<int>(), row["user_id"].as<int>(), row["friend_id"].as<int>(), row["status"].as<std::string>(), request_date, response_date);
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return friendships;
}

void FriendshipDao::deleteFriendship(int friendship_id) {
    try {
        pqxx::connection C(connection_string);
        pqxx::work W(C);
        std::string sql = "DELETE FROM friendships WHERE friendship_id = " + W.quote(friendship_id);
        W.exec(sql);
        W.commit();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

int FriendshipDao::createFriendRequest(int user_id, int friend_id) {
    try {
        pqxx::connection C(connection_string);
        pqxx::work W(C);
        std::time_t now = std::time(0);
        std::string sql = "INSERT INTO friendships (user_id, friend_id, status, request_date) VALUES (" +
                          W.quote(user_id) + ", " +
                          W.quote(friend_id) + ", " +
                          W.quote("pending") + ", " +
                          W.quote(formatTime(now)) + ") RETURNING friendship_id";
        pqxx::result R = W.exec(sql);
        W.commit();
        return R[0][0].as<int>();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        throw;
    }
}

void FriendshipDao::acceptFriendRequest(int friendship_id) {
    try {
        pqxx::connection C(connection_string);
        pqxx::work W(C);
        std::time_t now = std::time(0);
        std::string sql = "UPDATE friendships SET status = 'accepted', response_date = " + W.quote(formatTime(now)) +
                          " WHERE friendship_id = " + W.quote(friendship_id);
        W.exec(sql);
        W.commit();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        throw;
    }
}
