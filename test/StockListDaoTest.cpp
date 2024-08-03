//
// Created by xinyuli on 8/2/24.
//

#include <iostream>
#include "../src/dao/StockListDao.h"

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
        StockListDao stockListDao(conn_str);

        std::cout << "Connected to the database successfully!" << std::endl;

        // Add a new StockList
        StockList newList(1, "Tech Stocks", true);
        stockListDao.addStockList(newList);
        std::cout << "Added StockList with ID: " << newList.getListId() << std::endl;

        // Retrieve a StockList by ID
        StockList retrievedList = stockListDao.getStockListById(newList.getListId());
        std::cout << "Retrieved StockList: " << retrievedList.getName() << std::endl;

        // Update the StockList
        retrievedList.setName("Updated Tech Stocks");
        stockListDao.updateStockList(retrievedList);
        std::cout << "Updated StockList name to: " << retrievedList.getName() << std::endl;

//        // Delete the StockList
//        stockListDao.deleteStockList(retrievedList.getListId());
//        std::cout << "Deleted StockList with ID: " << retrievedList.getListId() << std::endl;

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
