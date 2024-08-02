#include "StockListDao.h"
#include <pqxx/pqxx>

StockListDao::StockListDao(const std::string& conn_str) : conn(conn_str) {}

void StockListDao::addStockList(StockList &stockList) {
    pqxx::work txn{conn};
    std::string query = "INSERT INTO stock_lists (user_id, name, is_public) VALUES (" +
                        txn.quote(stockList.getUserId()) + ", " +
                        txn.quote(stockList.getName()) + ", " +
                        txn.quote(stockList.getIsPublic()) +
                        ") RETURNING list_id";
    pqxx::result result = txn.exec(query);
    stockList.setListId(result[0][0].as<int>());
    txn.commit();
}

StockList StockListDao::getStockListById(int listId) {
    pqxx::work txn{conn};
    std::string query = "SELECT * FROM stock_lists WHERE list_id = " + txn.quote(listId);
    pqxx::result result = txn.exec(query);
    if (result.empty()) {
        throw std::runtime_error("StockList not found");
    }
    StockList stockList;
    stockList.setListId(result[0]["list_id"].as<int>());
    stockList.setUserId(result[0]["user_id"].as<int>());
    stockList.setName(result[0]["name"].as<std::string>());
    stockList.setIsPublic(result[0]["is_public"].as<bool>());
    return stockList;
}

std::vector<StockList> StockListDao::getStockListsByUserId(int userId) {
    pqxx::work txn{conn};
    std::string query = "SELECT * FROM stock_lists WHERE user_id = " + txn.quote(userId);
    pqxx::result result = txn.exec(query);
    std::vector<StockList> stockLists;
    for (const auto& row : result) {
        StockList stockList;
        stockList.setListId(row["list_id"].as<int>());
        stockList.setUserId(row["user_id"].as<int>());
        stockList.setName(row["name"].as<std::string>());
        stockList.setIsPublic(row["is_public"].as<bool>());
        stockLists.push_back(stockList);
    }
    return stockLists;
}

void StockListDao::updateStockList(const StockList &stockList) {
    pqxx::work txn{conn};
    std::string query = "UPDATE stock_lists SET user_id = " + txn.quote(stockList.getUserId()) +
                        ", name = " + txn.quote(stockList.getName()) +
                        ", is_public = " + txn.quote(stockList.getIsPublic()) +
                        " WHERE list_id = " + txn.quote(stockList.getListId());
    txn.exec(query);
    txn.commit();
}

void StockListDao::deleteStockList(int listId) {
    pqxx::work txn{conn};
    std::string query = "DELETE FROM stock_lists WHERE list_id = " + txn.quote(listId);
    txn.exec(query);
    txn.commit();
}
