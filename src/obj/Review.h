//
// Created by xinyuli on 8/2/24.
//

#ifndef CSCC43_STOCK_SOCIAL_NETWORK_REVIEW_H
#define CSCC43_STOCK_SOCIAL_NETWORK_REVIEW_H


#include <string>
#include <ctime>

class Review {
private:
    int review_id;
    int list_id;
    int user_id;
    std::string content;
    std::time_t review_date;

public:
    Review(int list_id, int user_id, const std::string& content, std::time_t review_date);
    Review(int review_id, int list_id, int user_id, const std::string& content, std::time_t review_date);

    int getReviewId() const;
    int getListId() const;
    int getUserId() const;
    std::string getContent() const;
    std::time_t getReviewDate() const;

    void setListId(int list_id);
    void setUserId(int user_id);
    void setContent(const std::string& content);
    void setReviewDate(std::time_t review_date);
};


#endif //CSCC43_STOCK_SOCIAL_NETWORK_REVIEW_H
