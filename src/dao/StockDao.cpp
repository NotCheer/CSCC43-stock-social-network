#include "StockDao.h"
#include <iostream>

StockDao::StockDao(pqxx::connection& conn) : conn(conn) {}

bool StockDao::createStock(const Stock& stock) {
    try {
        pqxx::work W(conn);
        W.exec_params("INSERT INTO stocks (symbol, company_name, current_price) VALUES (\$1, \$2, \$3)",
                      stock.getSymbol(), stock.getCompanyName(), stock.getCurrentPrice());
        W.commit();
        return true;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
}

Stock StockDao::getStockById(int stock_id) {
    try {
        pqxx::work W(conn);
        pqxx::result R = W.exec_params("SELECT stock_id, symbol, company_name, current_price FROM stocks WHERE stock_id = \$1", stock_id);
        W.commit();

        if (R.size() != 1) {
            throw std::runtime_error("Stock not found");
        }

        pqxx::row row = R[0];
        return Stock(row["stock_id"].as<int>(), row["symbol"].as<std::string>(), row["company_name"].as<std::string>(), row["current_price"].as<double>());
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        throw;
    }
}

bool StockDao::updateStock(const Stock& stock) {
    try {
        pqxx::work W(conn);
        W.exec_params("UPDATE stocks SET symbol = \$1, company_name = \$2, current_price = \$3 WHERE stock_id = \$4",
                      stock.getSymbol(), stock.getCompanyName(), stock.getCurrentPrice(), stock.getId());
        W.commit();
        return true;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
}

bool StockDao::deleteStock(int stock_id) {
    try {
        pqxx::work W(conn);
        W.exec_params("DELETE FROM stocks WHERE stock_id = \$1", stock_id);
        W.commit();
        return true;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
}

std::vector<Stock> StockDao::getAllStocks() {
    pqxx::work txn(conn);
    pqxx::result result = txn.exec("SELECT stock_id, symbol, company_name, current_price FROM stocks");

    std::vector<Stock> stocks;
    for (const auto& row : result) {
        Stock stock(
                row["stock_id"].as<int>(),
                row["symbol"].as<std::string>(),
                row["company_name"].as<std::string>(),
                row["current_price"].as<double>()
        );
        stocks.push_back(stock);
    }

    return stocks;
}