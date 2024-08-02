#include <iostream>
#include <pqxx/pqxx>
#include "PortfolioDao.h"
#include "PortfolioHoldingDao.h"

void initializeDatabase(pqxx::connection& conn);

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
        pqxx::connection conn(conn_str);

        std::cout << "Connected to the database successfully!" << std::endl;


        // Initialize DAOs
        PortfolioDao portfolioDao(conn);
        PortfolioHoldingDao portfolioHoldingDao(conn);

        // Create a new portfolio
        Portfolio newPortfolio(1, "Tech Stocks", 10000.0);
        portfolioDao.createPortfolio(newPortfolio);
        std::cout << "Created Portfolio ID: " << newPortfolio.getId() << std::endl;

        // Get portfolio by ID
        Portfolio fetchedPortfolio = portfolioDao.getPortfolioById(newPortfolio.getId());
        std::cout << "Fetched Portfolio: " << fetchedPortfolio.getName() << " with Cash Account: " << fetchedPortfolio.getCashAccount() << std::endl;

        // Update portfolio
        fetchedPortfolio.setName("Updated Tech Stocks");
        fetchedPortfolio.setCashAccount(12000.0);
        portfolioDao.updatePortfolio(fetchedPortfolio);
        std::cout << "Updated Portfolio ID: " << fetchedPortfolio.getId() << std::endl;

        // Get portfolios by user ID
        std::vector<Portfolio> userPortfolios = portfolioDao.getPortfoliosByUserId(1);
        std::cout << "User Portfolios: " << std::endl;
        for (const Portfolio& portfolio : userPortfolios) {
            std::cout << "Portfolio ID: " << portfolio.getId() << ", Name: " << portfolio.getName() << ", Cash Account: " << portfolio.getCashAccount() << std::endl;
        }

        // Add stock to portfolio
        PortfolioHolding newHolding(newPortfolio.getId(), 101, 50);
        portfolioHoldingDao.addStockToPortfolio(newHolding);
        std::cout << "Added Stock Holding ID: " << newHolding.getId() << std::endl;

        // Get portfolio holding by ID
        PortfolioHolding fetchedHolding = portfolioHoldingDao.getPortfolioHoldingById(newHolding.getId());
        std::cout << "Fetched Holding: Stock ID: " << fetchedHolding.getStockId() << ", Quantity: " << fetchedHolding.getQuantity() << std::endl;

        // Update portfolio holding
        fetchedHolding.setQuantity(75);
        portfolioHoldingDao.updatePortfolioHolding(fetchedHolding);
        std::cout << "Updated Holding ID: " << fetchedHolding.getId() << std::endl;

        // Get portfolio holdings by portfolio ID
        std::vector<PortfolioHolding> portfolioHoldings = portfolioHoldingDao.getPortfolioHoldingsByPortfolioId(newPortfolio.getId());
        std::cout << "Portfolio Holdings: " << std::endl;
        for (const PortfolioHolding& holding : portfolioHoldings) {
            std::cout << "Holding ID: " << holding.getId() << ", Stock ID: " << holding.getStockId() << ", Quantity: " << holding.getQuantity() << std::endl;
        }

        // Delete portfolio holding
        portfolioHoldingDao.deletePortfolioHolding(fetchedHolding.getId());
        std::cout << "Deleted Holding ID: " << fetchedHolding.getId() << std::endl;

        // Delete portfolio
        portfolioDao.deletePortfolio(newPortfolio.getId());
        std::cout << "Deleted Portfolio ID: " << newPortfolio.getId() << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

