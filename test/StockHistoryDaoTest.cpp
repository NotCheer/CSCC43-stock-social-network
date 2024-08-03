#include <iostream>
#include <pqxx/pqxx>
#include "../src/obj/StockHistory.h"
#include "../src/dao/StockHistoryDao.h"

void testAddStockHistory(StockHistoryDao& dao) {
    StockHistory stockHistory(0, "2024-08-02 12:00:00", 100.0, 110.0, 95.0, 105.0, 10000, "NNTH");
    dao.addStockHistory(stockHistory);
    std::cout << "Stock history added successfully." << std::endl;
}

void testUpdateStockHistory(StockHistoryDao& dao) {
    StockHistory stockHistory(619046, "2024-08-02 12:00:00", 100.0, 110.0, 95.0, 105.0, 10000, "NNTH");
    stockHistory.setClose(107.0); // Update the close price
    dao.updateStockHistory(stockHistory);
    std::cout << "Stock history updated successfully." << std::endl;
}

void testDeleteStockHistory(StockHistoryDao& dao) {
    dao.deleteStockHistory(619046);
    std::cout << "Stock history deleted successfully." << std::endl;
}

void testGetStockHistoryById(StockHistoryDao& dao) {
    try {
        StockHistory stockHistory = dao.getStockHistoryById(2);
        std::cout << "Retrieved stock history: " << stockHistory.getSymbol() << " at " << stockHistory.getTimestamp() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

void testGetStockHistoriesBySymbolAndDuration(StockHistoryDao& dao) {
    std::vector<StockHistory> stockHistories = dao.getStockHistoriesBySymbolAndDuration("NNTH", "2024-08-01 00:00:00", 7);
    std::cout << "Retrieved " << stockHistories.size() << " stock histories for NNTH within the specified duration." << std::endl;
}

void testGetStockHistoriesBySymbol(StockHistoryDao& dao) {
    std::vector<StockHistory> stockHistories = dao.getStockHistoriesBySymbol("NNTH");
    std::cout << "Retrieved " << stockHistories.size() << " stock histories for NNTH." << std::endl;
}

int main() {
    try {
        pqxx::connection conn("dbname=mydb user=postgres password=postgres hostaddr=127.0.0.1 port=5432");
        StockHistoryDao dao(conn);

        // Test adding a stock history
        testAddStockHistory(dao);

        // Test updating a stock history
        testUpdateStockHistory(dao);

        // Test getting a stock history by ID
        testGetStockHistoryById(dao);

        // Test getting stock histories by symbol and duration
        testGetStockHistoriesBySymbolAndDuration(dao);

        // Test getting stock histories by symbol
        testGetStockHistoriesBySymbol(dao);

        // Test deleting a stock history
        testDeleteStockHistory(dao);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
