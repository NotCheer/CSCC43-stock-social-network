#include <crow.h>
#include <pqxx/pqxx>
#include <iostream>
#include <nlohmann/json.hpp>
#include "../src/dao/PortfolioStatsCalculator.h"

int main() {
    try {
        std::string dbname = "mydb";
        std::string dbuser = "postgres";
        std::string dbpassword = "postgres";
        std::string dbhost = "127.0.0.1";
        std::string dbport = "5432";

        // Create connection string
        std::string conn_str = "dbname=" + dbname + " user=" + dbuser + " password=" + dbpassword +
                               " hostaddr=" + dbhost + " port=" + dbport;

        // Establish connection
        pqxx::connection conn(conn_str);

        PortfolioStatsCalculator calculator(conn);
        int portfolioId = 1; // Example portfolio ID
        calculator.calculateAndStorePortfolioStats(portfolioId);
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
