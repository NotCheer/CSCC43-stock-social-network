//
// Created by xinyuli on 8/2/24.
//

#ifndef CSCC43_STOCK_SOCIAL_NETWORK_REVIEWDAO_H
#define CSCC43_STOCK_SOCIAL_NETWORK_REVIEWDAO_H


#include <string>
#include <vector>
#include <pqxx/pqxx>
#include "../obj/Review.h"

class ReviewDao {
private:
    std::string connection_string;

public:
    ReviewDao(const std::string& conn_str);

    int addReview(const Review& review);
    Review getReviewById(int review_id);
    std::vector<Review> getReviewsByListId(int list_id);
    void updateReview(const Review& review);
    void deleteReview(int review_id);
};


#endif //CSCC43_STOCK_SOCIAL_NETWORK_REVIEWDAO_H
