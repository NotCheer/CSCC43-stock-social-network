//
// Created by xinyuli on 8/2/24.
//

#ifndef CSCC43_STOCK_SOCIAL_NETWORK_PORTFOLIOHOLDINGDAO_H
#define CSCC43_STOCK_SOCIAL_NETWORK_PORTFOLIOHOLDINGDAO_H


#include <pqxx/pqxx>
#include "../obj/PortfolioHolding.h"
#include <vector>

class PortfolioHoldingDao {
private:
    pqxx::connection& conn;

public:
    PortfolioHoldingDao(pqxx::connection& conn);

    void addStockToPortfolio(PortfolioHolding& portfolioHolding);
    PortfolioHolding getPortfolioHoldingById(int id);
    std::vector<PortfolioHolding> getPortfolioHoldingsByPortfolioId(int portfolioId);
    void updatePortfolioHolding(const PortfolioHolding& portfolioHolding);
    void deletePortfolioHolding(int id);
};


#endif //CSCC43_STOCK_SOCIAL_NETWORK_PORTFOLIOHOLDINGDAO_H
