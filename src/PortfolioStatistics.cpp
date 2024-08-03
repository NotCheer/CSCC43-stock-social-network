#include <vector>
#include <string>
#include <cmath>
#include <iostream>
#include <pqxx/pqxx>
#include <numeric>
#include <algorithm>
#include "dao/StockHistoryDao.h"

class PortfolioStatistics {
public:
    PortfolioStatistics(StockHistoryDao& dao) : dao(dao) {}

    void calculateStatistics(const std::vector<std::string>& symbols, const std::string& startTime, int duration, const std::string& marketSymbol) {
        std::vector<std::vector<double>> returns;
        std::vector<double> marketReturns;

        // Fetch market index data and calculate returns
        auto marketHistories = dao.getStockHistoriesBySymbolAndDuration(marketSymbol, startTime, duration);
        if (marketHistories.empty()) {
            std::cerr << "No market data found for symbol: " << marketSymbol << "\n";
            return;
        }
        marketReturns = calculateDailyReturns(marketHistories);

        for (const auto& symbol : symbols) {
            auto histories = dao.getStockHistoriesBySymbolAndDuration(symbol, startTime, duration);
            if (histories.empty()) {
                std::cerr << "No data found for symbol: " << symbol << "\n";
                continue;
            }
            std::vector<double> dailyReturns = calculateDailyReturns(histories);
            returns.push_back(dailyReturns);
        }

        if (returns.empty()) {
            std::cerr << "No valid stock data found.\n";
            return;
        }

        for (size_t i = 0; i < symbols.size(); ++i) {
            if (i >= returns.size()) {
                std::cerr << "Skipping symbol " << symbols[i] << " due to lack of returns data.\n";
                continue;
            }
            double mean = calculateMean(returns[i]);
            double stddev = calculateStdDev(returns[i], mean);
            double cv = stddev / mean;
            double beta = calculateBeta(returns[i], marketReturns);

            std::cout << "Stock: " << symbols[i] << "\n";
            std::cout << "Mean: " << mean << ", StdDev: " << stddev << ", Coefficient of Variation: " << cv << ", Beta: " << beta << "\n";
        }

        auto covarianceMatrix = calculateCovarianceMatrix(returns);
        auto correlationMatrix = calculateCorrelationMatrix(covarianceMatrix);

        // Print matrices
        printMatrix(covarianceMatrix, "Covariance Matrix");
        printMatrix(correlationMatrix, "Correlation Matrix");
    }

private:
    StockHistoryDao& dao;

    std::vector<double> calculateDailyReturns(const std::vector<StockHistory>& histories) {
        std::vector<double> returns;
        for (size_t i = 1; i < histories.size(); ++i) {
            double returnVal = (histories[i].close - histories[i-1].close) / histories[i-1].close;
            returns.push_back(returnVal);
        }
        return returns;
    }

    double calculateMean(const std::vector<double>& returns) {
        if (returns.empty()) return 0.0;
        return std::accumulate(returns.begin(), returns.end(), 0.0) / returns.size();
    }

    double calculateStdDev(const std::vector<double>& returns, double mean) {
        if (returns.size() < 2) return 0.0;
        double sum = 0.0;
        for (const auto& r : returns) {
            sum += (r - mean) * (r - mean);
        }
        return std::sqrt(sum / (returns.size() - 1));
    }

    double calculateBeta(const std::vector<double>& stockReturns, const std::vector<double>& marketReturns) {
        if (stockReturns.size() != marketReturns.size() || stockReturns.empty()) {
            std::cerr << "Mismatched or empty returns data for beta calculation.\n";
            return 0.0;
        }

        double meanStock = calculateMean(stockReturns);
        double meanMarket = calculateMean(marketReturns);

        double covariance = 0.0;
        double varianceMarket = 0.0;

        for (size_t i = 0; i < stockReturns.size(); ++i) {
            covariance += (stockReturns[i] - meanStock) * (marketReturns[i] - meanMarket);
            varianceMarket += (marketReturns[i] - meanMarket) * (marketReturns[i] - meanMarket);
        }

        covariance /= (stockReturns.size() - 1);
        varianceMarket /= (marketReturns.size() - 1);

        return covariance / varianceMarket;
    }

    std::vector<std::vector<double>> calculateCovarianceMatrix(const std::vector<std::vector<double>>& returns) {
        size_t n = returns.size();
        std::vector<std::vector<double>> covMatrix(n, std::vector<double>(n, 0.0));
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < n; ++j) {
                double mean_i = calculateMean(returns[i]);
                double mean_j = calculateMean(returns[j]);
                double sum = 0.0;
                for (size_t k = 0; k < returns[i].size(); ++k) {
                    sum += (returns[i][k] - mean_i) * (returns[j][k] - mean_j);
                }
                covMatrix[i][j] = sum / (returns[i].size() - 1);
            }
        }
        return covMatrix;
    }

    std::vector<std::vector<double>> calculateCorrelationMatrix(const std::vector<std::vector<double>>& covMatrix) {
        size_t n = covMatrix.size();
        std::vector<std::vector<double>> corrMatrix(n, std::vector<double>(n, 0.0));
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < n; ++j) {
                double stddev_i = std::sqrt(covMatrix[i][i]);
                double stddev_j = std::sqrt(covMatrix[j][j]);
                corrMatrix[i][j] = covMatrix[i][j] / (stddev_i * stddev_j);
            }
        }
        return corrMatrix;
    }

    void printMatrix(const std::vector<std::vector<double>>& matrix, const std::string& name) {
        std::cout << name << ":\n";
        for (const auto& row : matrix) {
            for (const auto& val : row) {
                std::cout << val << " ";
            }
            std::cout << "\n";
        }
    }
};

int main() {
    try {
        std::string dbname = "mydb";
        std::string dbuser = "postgres";
        std::string dbpassword = "postgres";
        std::string dbhost = "127.0.0.1";
        std::string dbport = "5432";

        // Create connection string
        std::string conn_str = "dbname=" + dbname + " user=" + dbuser + " password=" + dbpassword +
                               " hostaddr=" + dbhost + " port=" + dbport;

        // Establish connection
        pqxx::connection conn(conn_str);
        StockHistoryDao dao(conn);

        std::vector<std::string> symbols = {"AAPL", "GOOGL", "MSFT"};
        std::string startTime = "2014-01-01";
        int duration = 365; // 1 year
        std::string marketSymbol = "A"; // Example market index symbol

        PortfolioStatistics stats(dao);
        stats.calculateStatistics(symbols, startTime, duration, marketSymbol);
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}