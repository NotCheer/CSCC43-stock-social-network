#include <iostream>
#include "../src/dao/PortfolioStatDao.h"

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
        PortfolioStatDao portfolioStatDao(conn_str);

        std::cout << "Connected to the database successfully!" << std::endl;

        // Add a new PortfolioStat
        PortfolioStat newPortfolioStat(1, 0.5, 1.2, "1,0.5,0.5,1");
        int newPortfolioStatId = portfolioStatDao.addPortfolioStat(newPortfolioStat);
        std::cout << "Added PortfolioStat with ID: " << newPortfolioStatId << std::endl;

        // Retrieve a PortfolioStat by ID
        PortfolioStat retrievedPortfolioStat = portfolioStatDao.getPortfolioStatById(newPortfolioStatId);
        std::cout << "Retrieved PortfolioStat for Portfolio " << retrievedPortfolioStat.getPortfolioId() << " with COV " << retrievedPortfolioStat.getCov() << " and beta " << retrievedPortfolioStat.getBeta() << std::endl;

        // Get PortfolioStats by Portfolio ID
        std::vector<PortfolioStat> portfolioStats = portfolioStatDao.getPortfolioStatsByPortfolioId(1);
        std::cout << "PortfolioStats for Portfolio ID 1: " << std::endl;
        for (const auto& portfolioStat : portfolioStats) {
            std::cout << "PortfolioStat ID: " << portfolioStat.getStatId() << ", COV: " << portfolioStat.getCov() << ", Beta: " << portfolioStat.getBeta() << std::endl;
        }

        // Update the PortfolioStat
        retrievedPortfolioStat.setCov(0.6);
        retrievedPortfolioStat.setBeta(1.3);
        retrievedPortfolioStat.setCovMatrix("1,0.6,0.6,1");
        portfolioStatDao.updatePortfolioStat(retrievedPortfolioStat);
        std::cout << "Updated PortfolioStat with ID: " << retrievedPortfolioStat.getStatId() << std::endl;

        // Delete the PortfolioStat
        portfolioStatDao.deletePortfolioStat(newPortfolioStatId);
        std::cout << "Deleted PortfolioStat with ID: " << newPortfolioStatId << std::endl;

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
