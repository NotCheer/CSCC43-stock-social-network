#include <iostream>
#include "../src/dao/ReviewDao.h"

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
        ReviewDao reviewDao(conn_str);

        std::cout << "Connected to the database successfully!" << std::endl;

        // Add a new Review
        std::time_t now = std::time(0);
        Review newReview(1, 1, "Great stock list!", now);
        int newReviewId = reviewDao.addReview(newReview);
        std::cout << "Added Review with ID: " << newReviewId << std::endl;

        // Retrieve a Review by ID
        Review retrievedReview = reviewDao.getReviewById(newReviewId);
        std::cout << "Retrieved Review: " << retrievedReview.getContent() << std::endl;

        // Update the Review
        retrievedReview.setContent("Updated review content.");
        reviewDao.updateReview(retrievedReview);
        std::cout << "Updated Review content to: " << retrievedReview.getContent() << std::endl;

        // Get Reviews by List ID
        std::vector<Review> reviews = reviewDao.getReviewsByListId(1);
        std::cout << "Reviews for List ID 1: " << std::endl;
        for (const auto& review : reviews) {
            std::cout << "Review ID: " << review.getReviewId() << ", Content: " << review.getContent() << std::endl;
        }

        // Delete the Review
        reviewDao.deleteReview(newReviewId);
        std::cout << "Deleted Review with ID: " << newReviewId << std::endl;

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
