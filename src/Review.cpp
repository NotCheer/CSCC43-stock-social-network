#include "Review.h"

Review::Review(int list_id, int user_id, const std::string& content, std::time_t review_date)
        : list_id(list_id), user_id(user_id), content(content), review_date(review_date) {}

Review::Review(int review_id, int list_id, int user_id, const std::string& content, std::time_t review_date)
        : review_id(review_id), list_id(list_id), user_id(user_id), content(content), review_date(review_date) {}

int Review::getReviewId() const {
    return review_id;
}

int Review::getListId() const {
    return list_id;
}

int Review::getUserId() const {
    return user_id;
}

std::string Review::getContent() const {
    return content;
}

std::time_t Review::getReviewDate() const {
    return review_date;
}

void Review::setListId(int list_id) {
    this->list_id = list_id;
}

void Review::setUserId(int user_id) {
    this->user_id = user_id;
}

void Review::setContent(const std::string& content) {
    this->content = content;
}

void Review::setReviewDate(std::time_t review_date) {
    this->review_date = review_date;
}
