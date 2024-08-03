//
// Created by xinyuli on 8/2/24.
//

#ifndef CSCC43_STOCK_SOCIAL_NETWORK_PORTFOLIOSTATDAO_H
#define CSCC43_STOCK_SOCIAL_NETWORK_PORTFOLIOSTATDAO_H


#include <string>
#include <vector>
#include <pqxx/pqxx>
#include "../obj/PortfolioStat.h"

class PortfolioStatDao {
private:
    std::string connection_string;

public:
    PortfolioStatDao(const std::string& conn_str);

    int addPortfolioStat(const PortfolioStat& portfolio_stat);
    PortfolioStat getPortfolioStatById(int stat_id);
    std::vector<PortfolioStat> getPortfolioStatsByPortfolioId(int portfolio_id);
    void updatePortfolioStat(const PortfolioStat& portfolio_stat);
    void deletePortfolioStat(int stat_id);
};


#endif //CSCC43_STOCK_SOCIAL_NETWORK_PORTFOLIOSTATDAO_H
