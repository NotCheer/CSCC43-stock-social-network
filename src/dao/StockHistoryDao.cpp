#include "StockHistoryDao.h"
#include "../dao/StockDao.h"
#include <ctime>
#include <iomanip>
#include <sstream>

StockHistoryDao::StockHistoryDao(pqxx::connection& conn) : conn(conn) {}

void StockHistoryDao::addStockHistory(const StockHistory& stockHistory) {
    pqxx::work w(conn);
    w.exec0("INSERT INTO stock_history (timestamp, open, high, low, close, volume, symbol) VALUES (" +
            w.quote(stockHistory.getTimestamp()) + ", " +
            w.quote(stockHistory.getOpen()) + ", " +
            w.quote(stockHistory.getHigh()) + ", " +
            w.quote(stockHistory.getLow()) + ", " +
            w.quote(stockHistory.getClose()) + ", " +
            w.quote(stockHistory.getVolume()) + ", " +
            w.quote(stockHistory.getSymbol()) + ")");
    w.commit();
}

void StockHistoryDao::updateStockHistory(const StockHistory& stockHistory) {
    pqxx::work w(conn);
    w.exec0("UPDATE stock_history SET timestamp = " + w.quote(stockHistory.getTimestamp()) +
            ", open = " + w.quote(stockHistory.getOpen()) +
            ", high = " + w.quote(stockHistory.getHigh()) +
            ", low = " + w.quote(stockHistory.getLow()) +
            ", close = " + w.quote(stockHistory.getClose()) +
            ", volume = " + w.quote(stockHistory.getVolume()) +
            ", symbol = " + w.quote(stockHistory.getSymbol()) +
            " WHERE stock_history_id = " + w.quote(stockHistory.getStockHistoryId()));
    w.commit();
}

void StockHistoryDao::deleteStockHistory(int stockHistoryId) {
    pqxx::work w(conn);
    w.exec0("DELETE FROM stock_history WHERE stock_history_id = " + w.quote(stockHistoryId));
    w.commit();
}

StockHistory StockHistoryDao::getStockHistoryById(int stockHistoryId) {
    pqxx::work w(conn);
    pqxx::result r = w.exec("SELECT * FROM stock_history WHERE stock_history_id = " + w.quote(stockHistoryId));
    w.commit();

    if (r.size() != 1) {
        throw std::runtime_error("Stock history not found");
    }

    const auto& row = r[0];
    return StockHistory(row["stock_history_id"].as<int>(),
                        row["timestamp"].as<std::string>(),
                        row["open"].as<double>(),
                        row["high"].as<double>(),
                        row["low"].as<double>(),
                        row["close"].as<double>(),
                        row["volume"].as<long>(),
                        row["symbol"].as<std::string>());
}

std::vector<StockHistory> StockHistoryDao::getStockHistoriesBySymbolAndDuration(const std::string& symbol, const std::string& startTime, int duration) {
    // Parse the start time
    std::tm tm = {};
    std::istringstream ss(startTime);
    ss >> std::get_time(&tm, "%Y-%m-%d");
    if (ss.fail()) {
        throw std::runtime_error("Failed to parse start time");
    }

    // Calculate the end time
    std::time_t start_time_t = std::mktime(&tm);
    std::time_t end_time_t = start_time_t + duration * 24 * 60 * 60; // duration days later

    // Convert end time back to string in the same format
    std::tm end_tm = *std::localtime(&end_time_t);
    std::ostringstream end_ss;
    end_ss << std::put_time(&end_tm, "%Y-%m-%d");
    std::string endTime = end_ss.str();

    pqxx::work w(conn);
    std::string query = "SELECT * FROM stock_history WHERE symbol = " + w.quote(symbol) +
                        " AND timestamp >= " + w.quote(startTime) +
                        " AND timestamp < " + w.quote(endTime);
    pqxx::result r = w.exec(query);
    w.commit();

    std::vector<StockHistory> stockHistories;
    for (const auto& row : r) {
        stockHistories.emplace_back(row["stock_history_id"].as<int>(),
                                    row["timestamp"].as<std::string>(),
                                    row["open"].as<double>(),
                                    row["high"].as<double>(),
                                    row["low"].as<double>(),
                                    row["close"].as<double>(),
                                    row["volume"].as<long>(),
                                    row["symbol"].as<std::string>());
    }

    return stockHistories;
}

std::vector<StockHistory> StockHistoryDao::getStockHistoriesBySymbol(const std::string& symbol) {
    pqxx::work w(conn);
    pqxx::result r = w.exec("SELECT * FROM stock_history WHERE symbol = " + w.quote(symbol));
    w.commit();

    std::vector<StockHistory> stockHistories;
    for (const auto& row : r) {
        stockHistories.emplace_back(row["stock_history_id"].as<int>(),
                                    row["timestamp"].as<std::string>(),
                                    row["open"].as<double>(),
                                    row["high"].as<double>(),
                                    row["low"].as<double>(),
                                    row["close"].as<double>(),
                                    row["volume"].as<long>(),
                                    row["symbol"].as<std::string>());
    }

    return stockHistories;
}

std::vector<StockHistory> StockHistoryDao::getStockHistoriesByStockId(int stockId) {
    pqxx::work txn(conn);
    StockDao stockDao(conn);
    Stock stock = stockDao.getStockById(stockId);
    std::string symbol = stock.getSymbol();

    pqxx::result result = txn.exec("SELECT * FROM stock_histories WHERE symbol = " + txn.quote(symbol) + " ORDER BY timestamp DESC");

    std::vector<StockHistory> stockHistories;
    for (const auto& row : result) {
        StockHistory stockHistory(
                row["stock_history_id"].as<int>(),
                row["timestamp"].as<std::string>(),
                row["open"].as<double>(),
                row["high"].as<double>(),
                row["low"].as<double>(),
                row["close"].as<double>(),
                row["volume"].as<long>(),
                row["symbol"].as<std::string>()
        );
        stockHistories.push_back(stockHistory);
    }

    return stockHistories;
}