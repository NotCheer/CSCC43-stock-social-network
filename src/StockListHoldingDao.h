//
// Created by xinyuli on 8/2/24.
//

#ifndef CSCC43_STOCK_SOCIAL_NETWORK_STOCKLISTHOLDINGDAO_H
#define CSCC43_STOCK_SOCIAL_NETWORK_STOCKLISTHOLDINGDAO_H


#include <string>
#include <vector>
#include <pqxx/pqxx>
#include "StockListHolding.h"

class StockListHoldingDao {
private:
    std::string connection_string;

public:
    StockListHoldingDao(const std::string& conn_str);

    int addStockListHolding(const StockListHolding& stockListHolding);
    StockListHolding getStockListHoldingById(int list_holding_id);
    std::vector<StockListHolding> getStockListHoldingsByListId(int list_id);
    void updateStockListHolding(const StockListHolding& stockListHolding);
    void deleteStockListHolding(int list_holding_id);
};


#endif //CSCC43_STOCK_SOCIAL_NETWORK_STOCKLISTHOLDINGDAO_H
