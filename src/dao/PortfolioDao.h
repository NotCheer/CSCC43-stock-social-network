//
// Created by xinyuli on 8/2/24.
//

#ifndef CSCC43_STOCK_SOCIAL_NETWORK_PORTFOLIODAO_H
#define CSCC43_STOCK_SOCIAL_NETWORK_PORTFOLIODAO_H


#include <pqxx/pqxx>
#include "../obj/Portfolio.h"
#include <vector>

class PortfolioDao {
private:
    pqxx::connection& conn;

public:
    PortfolioDao(pqxx::connection& conn);

    void createPortfolio(Portfolio& portfolio);
    Portfolio getPortfolioById(int id);
    std::vector<Portfolio> getPortfoliosByUserId(int userId);
    void updatePortfolio(const Portfolio& portfolio);
    void deletePortfolio(int id);
};

#endif //CSCC43_STOCK_SOCIAL_NETWORK_PORTFOLIODAO_H
