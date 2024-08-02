#include "StockListHoldingDao.h"
#include <iostream>

StockListHoldingDao::StockListHoldingDao(const std::string& conn_str) : connection_string(conn_str) {}

int StockListHoldingDao::addStockListHolding(const StockListHolding& stockListHolding) {
    int stockListHoldingId = -1;

    try {
        pqxx::connection C(connection_string);
        pqxx::work W(C);
        std::string sql = "INSERT INTO stock_list_holdings (list_id, stock_id, shares) VALUES (" +
                          W.quote(stockListHolding.getListId()) + ", " +
                          W.quote(stockListHolding.getStockId()) + ", " +
                          W.quote(stockListHolding.getShares()) + ") RETURNING list_holding_id";
        pqxx::result R = W.exec(sql);

        if (!R.empty()) {
            stockListHoldingId = R[0][0].as<int>();
        }

        W.commit();
        return stockListHoldingId;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
}

StockListHolding StockListHoldingDao::getStockListHoldingById(int list_holding_id) {
    try {
        pqxx::connection C(connection_string);
        pqxx::nontransaction N(C);
        std::string sql = "SELECT list_holding_id, list_id, stock_id, shares FROM stock_list_holdings WHERE list_holding_id = " + N.quote(list_holding_id);
        pqxx::result R(N.exec(sql));

        if (R.size() != 1) {
            throw std::runtime_error("StockListHolding not found");
        }

        pqxx::row row = R[0];
        return StockListHolding(row["list_holding_id"].as<int>(), row["list_id"].as<int>(), row["stock_id"].as<int>(), row["shares"].as<int>());
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        throw;
    }
}

std::vector<StockListHolding> StockListHoldingDao::getStockListHoldingsByListId(int list_id) {
    std::vector<StockListHolding> holdings;
    try {
        pqxx::connection C(connection_string);
        pqxx::nontransaction N(C);
        std::string sql = "SELECT list_holding_id, list_id, stock_id, shares FROM stock_list_holdings WHERE list_id = " + N.quote(list_id);
        pqxx::result R(N.exec(sql));

        for (auto row : R) {
            holdings.emplace_back(row["list_holding_id"].as<int>(), row["list_id"].as<int>(), row["stock_id"].as<int>(), row["shares"].as<int>());
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return holdings;
}

void StockListHoldingDao::updateStockListHolding(const StockListHolding& stockListHolding) {
    try {
        pqxx::connection C(connection_string);
        pqxx::work W(C);
        std::string sql = "UPDATE stock_list_holdings SET list_id = " + W.quote(stockListHolding.getListId()) +
                          ", stock_id = " + W.quote(stockListHolding.getStockId()) +
                          ", shares = " + W.quote(stockListHolding.getShares()) +
                          " WHERE list_holding_id = " + W.quote(stockListHolding.getListHoldingId());
        W.exec(sql);
        W.commit();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

void StockListHoldingDao::deleteStockListHolding(int list_holding_id) {
    try {
        pqxx::connection C(connection_string);
        pqxx::work W(C);
        std::string sql = "DELETE FROM stock_list_holdings WHERE list_holding_id = " + W.quote(list_holding_id);
        W.exec(sql);
        W.commit();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}
