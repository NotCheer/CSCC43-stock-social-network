//
// Created by xinyuli on 8/3/24.
//

#ifndef CSCC43_STOCK_SOCIAL_NETWORK_PORTFOLIOSTATSCALCULATOR_H
#define CSCC43_STOCK_SOCIAL_NETWORK_PORTFOLIOSTATSCALCULATOR_H


#include <pqxx/pqxx>
#include <vector>
#include <string>
#include "../dao/PortfolioHoldingDao.h"
#include "../dao/StockHistoryDao.h"
#include "../dao/StockDao.h"
#include "../obj/StockHistory.h"
#include "../obj/Stock.h"

class PortfolioStatsCalculator {
public:
    PortfolioStatsCalculator(pqxx::connection& conn);

    void calculateAndStorePortfolioStats(int portfolioId);

private:
    pqxx::connection& conn;
    PortfolioHoldingDao portfolioHoldingDao;
    StockHistoryDao stockHistoryDao;
    StockDao stockDao;

    std::vector<double> calculateCoefficientOfVariation(const std::vector<std::vector<double>>& stockPrices);
    std::vector<double> calculateBetaValues(const std::vector<std::vector<double>>& stockPrices, const std::vector<double>& marketPrices);
    std::vector<std::vector<double>> calculateCovarianceMatrix(const std::vector<std::vector<double>>& stockPrices);
    std::vector<std::vector<double>> calculateCorrelationMatrix(const std::vector<std::vector<double>>& covMatrix);
};


#endif //CSCC43_STOCK_SOCIAL_NETWORK_PORTFOLIOSTATSCALCULATOR_H
