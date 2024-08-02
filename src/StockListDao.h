//
// Created by xinyuli on 8/2/24.
//

#ifndef CSCC43_STOCK_SOCIAL_NETWORK_STOCKLISTDAO_H
#define CSCC43_STOCK_SOCIAL_NETWORK_STOCKLISTDAO_H


#include <string>
#include <vector>
#include "StockList.h"
#include <pqxx/pqxx>

class StockListDao {
private:
    pqxx::connection conn;

public:
    StockListDao(const std::string& conn_str);

    void addStockList(StockList &stockList);
    StockList getStockListById(int listId);
    std::vector<StockList> getStockListsByUserId(int userId);
    void updateStockList(const StockList &stockList);
    void deleteStockList(int listId);
};


#endif //CSCC43_STOCK_SOCIAL_NETWORK_STOCKLISTDAO_H
