#include <iostream>
#include <pqxx/pqxx>
#include "../src/Stock.h"
#include "../src/StockDao.h"

void testStockDao() {
    // Database connection parameters
    std::string dbname = "mydb";
    std::string dbuser = "postgres";
    std::string dbpassword = "postgres";
    std::string dbhost = "127.0.0.1";
    std::string dbport = "5432";

    // Create connection string
    std::string conn_str = "dbname=" + dbname + " user=" + dbuser + " password=" + dbpassword +
                           " hostaddr=" + dbhost + " port=" + dbport;

    try {
        pqxx::connection C(conn_str);
        if (C.is_open()) {
            std::cout << "Opened database successfully: " << C.dbname() << std::endl;

            StockDao stockDao(C);

            // Create a new stock
            Stock newStock(0, "AAPL", "Apple Inc.", 150.00); // ID is not needed here
            if (stockDao.createStock(newStock)) {
                std::cout << "Stock created successfully." << std::endl;
            } else {
                std::cerr << "Failed to create stock." << std::endl;
            }

            // Retrieve the stock by ID
            try {
                Stock retrievedStock = stockDao.getStockById(1);
                std::cout << "Retrieved Stock: " << retrievedStock.getSymbol() << ", " << retrievedStock.getCompanyName() << std::endl;
            } catch (const std::exception &e) {
                std::cerr << "Failed to retrieve stock: " << e.what() << std::endl;
            }

            // Update the stock
            try {
                Stock retrievedStock = stockDao.getStockById(1);
                retrievedStock.setCurrentPrice(155.00);
                if (stockDao.updateStock(retrievedStock)) {
                    std::cout << "Stock updated successfully." << std::endl;
                } else {
                    std::cerr << "Failed to update stock." << std::endl;
                }
            } catch (const std::exception &e) {
                std::cerr << "Failed to update stock: " << e.what() << std::endl;
            }

            // Delete the stock
            if (stockDao.deleteStock(1)) {
                std::cout << "Stock deleted successfully." << std::endl;
            } else {
                std::cerr << "Failed to delete stock." << std::endl;
            }

            C.disconnect();
        } else {
            std::cerr << "Can't open database" << std::endl;
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

int main() {
    testStockDao();
    return 0;
}
