#include "ReviewDao.h"
#include <iostream>
#include <iomanip>
#include <sstream>

ReviewDao::ReviewDao(const std::string& conn_str) : connection_string(conn_str) {}

std::string formatTime(std::time_t time) {
    std::tm *tm = std::gmtime(&time);
    std::stringstream ss;
    ss << std::put_time(tm, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

int ReviewDao::addReview(const Review& review) {
    try {
        pqxx::connection C(connection_string);
        pqxx::work W(C);
        std::string sql = "INSERT INTO reviews (list_id, user_id, content, review_date) VALUES (" +
                          W.quote(review.getListId()) + ", " +
                          W.quote(review.getUserId()) + ", " +
                          W.quote(review.getContent()) + ", " +
                          W.quote(formatTime(review.getReviewDate())) + ") RETURNING review_id";
        pqxx::result R = W.exec(sql);
        W.commit();
        return R[0][0].as<int>();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        throw;
    }
}

Review ReviewDao::getReviewById(int review_id) {
    try {
        pqxx::connection C(connection_string);
        pqxx::nontransaction N(C);
        std::string sql = "SELECT review_id, list_id, user_id, content, review_date FROM reviews WHERE review_id = " + N.quote(review_id);
        pqxx::result R(N.exec(sql));

        if (R.size() != 1) {
            throw std::runtime_error("Review not found");
        }

        pqxx::row row = R[0];
        std::tm tm = {};
        std::istringstream ss(row["review_date"].as<std::string>());
        ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
        std::time_t review_date = std::mktime(&tm);

        return Review(row["review_id"].as<int>(), row["list_id"].as<int>(), row["user_id"].as<int>(), row["content"].as<std::string>(), review_date);
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        throw;
    }
}

std::vector<Review> ReviewDao::getReviewsByListId(int list_id) {
    std::vector<Review> reviews;
    try {
        pqxx::connection C(connection_string);
        pqxx::nontransaction N(C);
        std::string sql = "SELECT review_id, list_id, user_id, content, review_date FROM reviews WHERE list_id = " + N.quote(list_id);
        pqxx::result R(N.exec(sql));

        for (auto row : R) {
            std::tm tm = {};
            std::istringstream ss(row["review_date"].as<std::string>());
            ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
            std::time_t review_date = std::mktime(&tm);

            reviews.emplace_back(row["review_id"].as<int>(), row["list_id"].as<int>(), row["user_id"].as<int>(), row["content"].as<std::string>(), review_date);
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return reviews;
}

void ReviewDao::updateReview(const Review& review) {
    try {
        pqxx::connection C(connection_string);
        pqxx::work W(C);
        std::string sql = "UPDATE reviews SET list_id = " + W.quote(review.getListId()) +
                          ", user_id = " + W.quote(review.getUserId()) +
                          ", content = " + W.quote(review.getContent()) +
                          ", review_date = " + W.quote(formatTime(review.getReviewDate())) +
                          " WHERE review_id = " + W.quote(review.getReviewId());
        W.exec(sql);
        W.commit();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

void ReviewDao::deleteReview(int review_id) {
    try {
        pqxx::connection C(connection_string);
        pqxx::work W(C);
        std::string sql = "DELETE FROM reviews WHERE review_id = " + W.quote(review_id);
        W.exec(sql);
        W.commit();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}
