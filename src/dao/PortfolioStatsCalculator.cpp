#include "PortfolioStatsCalculator.h"
#include <cmath>
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

PortfolioStatsCalculator::PortfolioStatsCalculator(pqxx::connection& conn)
        : conn(conn), portfolioHoldingDao(conn), stockHistoryDao(conn), stockDao(conn) {}

void PortfolioStatsCalculator::calculateAndStorePortfolioStats(int portfolioId) {
    // Step 1: Retrieve portfolio holdings
    std::vector<PortfolioHolding> holdings = portfolioHoldingDao.getPortfolioHoldingsByPortfolioId(portfolioId);
    std::vector<std::string> symbols;
    for (const auto& holding : holdings) {
        Stock stock = stockDao.getStockById(holding.getStockId());
        symbols.push_back(stock.getSymbol());
    }

    // Step 2: Fetch historical data
    std::vector<std::vector<double>> stockPrices;
    for (const auto& symbol : symbols) {
        std::vector<StockHistory> histories = stockHistoryDao.getStockHistoriesBySymbol(symbol);
        std::vector<double> prices;
        for (const auto& history : histories) {
            prices.push_back(history.getClose());
        }
        stockPrices.push_back(prices);
    }

    // Fetch historical data for the market index (e.g., "SPY")
    std::vector<StockHistory> marketHistories = stockHistoryDao.getStockHistoriesBySymbol("SPY");
    std::vector<double> marketPrices;
    for (const auto& history : marketHistories) {
        marketPrices.push_back(history.getClose());
    }

    // Step 3: Calculate statistics
    std::vector<double> coeffVariation = calculateCoefficientOfVariation(stockPrices);
    std::vector<double> betaValues = calculateBetaValues(stockPrices, marketPrices);
    std::vector<std::vector<double>> covMatrix = calculateCovarianceMatrix(stockPrices);
    std::vector<std::vector<double>> corrMatrix = calculateCorrelationMatrix(covMatrix);

    // Step 4: Store the statistics
    try {
        pqxx::work txn(conn);

        json coeffVariationJson = coeffVariation;
        json betaValuesJson = betaValues;
        json covMatrixJson = covMatrix;
        json corrMatrixJson = corrMatrix;

        std::string query = "INSERT INTO portfolio_stats (portfolio_id, cov, beta, coefficient_of_variation, beta_values, cov_matrix, correlation_matrix) VALUES (" +
                            txn.quote(portfolioId) + ", " +
                            txn.quote(0.0) + ", " + // Placeholder for cov and beta
                            txn.quote(0.0) + ", " +
                            txn.quote(coeffVariationJson.dump()) + ", " +
                            txn.quote(betaValuesJson.dump()) + ", " +
                            txn.quote(covMatrixJson.dump()) + ", " +
                            txn.quote(corrMatrixJson.dump()) + ")";

        txn.exec(query);
        txn.commit();
    } catch (const std::exception& e) {
        std::cerr << "Error inserting portfolio stats: " << e.what() << "\n";
    }
}

std::vector<double> PortfolioStatsCalculator::calculateCoefficientOfVariation(const std::vector<std::vector<double>>& stockPrices) {
    std::vector<double> coeffVariation;
    for (const auto& prices : stockPrices) {
        double mean = std::accumulate(prices.begin(), prices.end(), 0.0) / prices.size();
        double sq_sum = std::inner_product(prices.begin(), prices.end(), prices.begin(), 0.0);
        double stdev = std::sqrt(sq_sum / prices.size() - mean * mean);
        coeffVariation.push_back(stdev / mean);
    }
    return coeffVariation;
}

std::vector<double> PortfolioStatsCalculator::calculateBetaValues(const std::vector<std::vector<double>>& stockPrices, const std::vector<double>& marketPrices) {
    std::vector<double> betaValues;
    // Calculate beta for each stock
    for (const auto& prices : stockPrices) {
        double cov = 0.0;
        double varMarket = 0.0;
        double meanStock = std::accumulate(prices.begin(), prices.end(), 0.0) / prices.size();
        double meanMarket = std::accumulate(marketPrices.begin(), marketPrices.end(), 0.0) / marketPrices.size();
        for (size_t i = 0; i < prices.size(); ++i) {
            cov += (prices[i] - meanStock) * (marketPrices[i] - meanMarket);
            varMarket += (marketPrices[i] - meanMarket) * (marketPrices[i] - meanMarket);
        }
        betaValues.push_back(cov / varMarket);
    }
    return betaValues;
}

std::vector<std::vector<double>> PortfolioStatsCalculator::calculateCovarianceMatrix(const std::vector<std::vector<double>>& stockPrices) {
    size_t n = stockPrices.size();
    std::vector<std::vector<double>> covMatrix(n, std::vector<double>(n, 0.0));
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            double mean_i = std::accumulate(stockPrices[i].begin(), stockPrices[i].end(), 0.0) / stockPrices[i].size();
            double mean_j = std::accumulate(stockPrices[j].begin(), stockPrices[j].end(), 0.0) / stockPrices[j].size();
            for (size_t k = 0; k < stockPrices[i].size(); ++k) {
                covMatrix[i][j] += (stockPrices[i][k] - mean_i) * (stockPrices[j][k] - mean_j);
            }
            covMatrix[i][j] /= stockPrices[i].size();
        }
    }
    return covMatrix;
}

std::vector<std::vector<double>> PortfolioStatsCalculator::calculateCorrelationMatrix(const std::vector<std::vector<double>>& covMatrix) {
    size_t n = covMatrix.size();
    std::vector<std::vector<double>> corrMatrix(n, std::vector<double>(n, 0.0));
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            corrMatrix[i][j] = covMatrix[i][j] / std::sqrt(covMatrix[i][i] * covMatrix[j][j]);
        }
    }
    return corrMatrix;
}
