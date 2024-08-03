//
// Created by xinyuli on 8/3/24.
//

#ifndef CSCC43_STOCK_SOCIAL_NETWORK_STOCKHISTORYDAO_H
#define CSCC43_STOCK_SOCIAL_NETWORK_STOCKHISTORYDAO_H


#include <pqxx/pqxx>
#include <vector>
#include "../obj/StockHistory.h"

class StockHistoryDao {
public:
    StockHistoryDao(pqxx::connection& conn);

    void addStockHistory(const StockHistory& stockHistory);
    void updateStockHistory(const StockHistory& stockHistory);
    void deleteStockHistory(int stockHistoryId);
    StockHistory getStockHistoryById(int stockHistoryId);
    std::vector<StockHistory> getStockHistoriesBySymbolAndDuration(const std::string& symbol, const std::string& startTime, int duration);
    std::vector<StockHistory> getStockHistoriesBySymbol(const std::string& symbol);
    std::vector<StockHistory> getStockHistoriesByStockId(int stockId);

private:
    pqxx::connection& conn;
};


#endif //CSCC43_STOCK_SOCIAL_NETWORK_STOCKHISTORYDAO_H
