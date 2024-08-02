//
// Created by xinyuli on 8/1/24.
//

#ifndef CSCC43_STOCK_SOCIAL_NETWORK_STOCKDAO_H
#define CSCC43_STOCK_SOCIAL_NETWORK_STOCKDAO_H


#include <pqxx/pqxx>
#include "../obj/Stock.h"

class StockDao {
private:
    pqxx::connection& conn;

public:
    StockDao(pqxx::connection& conn);

    bool createStock(const Stock& stock);
    Stock getStockById(int id);
    bool updateStock(const Stock& stock);
    bool deleteStock(int id);
};


#endif //CSCC43_STOCK_SOCIAL_NETWORK_STOCKDAO_H
