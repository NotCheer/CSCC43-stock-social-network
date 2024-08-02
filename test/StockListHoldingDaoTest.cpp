#include <iostream>
#include "../src/StockListHoldingDao.h"

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
        StockListHoldingDao stockListHoldingDao(conn_str);

        std::cout << "Connected to the database successfully!" << std::endl;

        // Add a new StockListHolding
        StockListHolding newHolding(1, 1, 100);
        int id = stockListHoldingDao.addStockListHolding(newHolding);
        std::cout << "Added StockListHolding with List ID: " << id << std::endl;

        // Retrieve a StockListHolding by ID
        StockListHolding retrievedHolding = stockListHoldingDao.getStockListHoldingById(id);
        std::cout << "Retrieved StockListHolding: " << retrievedHolding.getShares() << " shares" << std::endl;

        // Update the StockListHolding
        retrievedHolding.setShares(200);
        stockListHoldingDao.updateStockListHolding(retrievedHolding);
        std::cout << "Updated StockListHolding shares to: " << retrievedHolding.getShares() << std::endl;

//        // Delete the StockListHolding
//        stockListHoldingDao.deleteStockListHolding(retrievedHolding.getListHoldingId());
//        std::cout << "Deleted StockListHolding with ID: " << retrievedHolding.getListHoldingId() << std::endl;

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
