#include "PortfolioHoldingDao.h"

PortfolioHoldingDao::PortfolioHoldingDao(pqxx::connection& conn) : conn(conn) {}

void PortfolioHoldingDao::addStockToPortfolio(PortfolioHolding& portfolioHolding) {
    pqxx::work txn(conn);
    pqxx::result result = txn.exec_params(
            "INSERT INTO portfolio_holdings (portfolio_id, stock_id, quantity) VALUES (\$1, \$2, \$3) RETURNING holding_id",
            portfolioHolding.getPortfolioId(), portfolioHolding.getStockId(), portfolioHolding.getQuantity()
    );
    portfolioHolding.setId(result[0][0].as<int>());
    txn.commit();
}

PortfolioHolding PortfolioHoldingDao::getPortfolioHoldingById(int id) {
    pqxx::work txn(conn);
    pqxx::result result = txn.exec_params("SELECT * FROM portfolio_holdings WHERE holding_id = \$1", id);

    if (result.empty()) {
        throw std::runtime_error("Portfolio holding not found");
    }

    const auto& row = result[0];
    PortfolioHolding portfolioHolding(row["portfolio_id"].as<int>(), row["stock_id"].as<int>(), row["quantity"].as<double>());
    portfolioHolding.setId(row["holding_id"].as<int>());
    return portfolioHolding;
}

std::vector<PortfolioHolding> PortfolioHoldingDao::getPortfolioHoldingsByPortfolioId(int portfolioId) {
    pqxx::work txn(conn);
    pqxx::result result = txn.exec_params("SELECT * FROM portfolio_holdings WHERE portfolio_id = \$1", portfolioId);

    std::vector<PortfolioHolding> holdings;
    for (const auto& row : result) {
        PortfolioHolding portfolioHolding(row["portfolio_id"].as<int>(), row["stock_id"].as<int>(), row["quantity"].as<double>());
        portfolioHolding.setId(row["holding_id"].as<int>());
        holdings.push_back(portfolioHolding);
    }

    return holdings;
}

void PortfolioHoldingDao::updatePortfolioHolding(const PortfolioHolding& portfolioHolding) {
    pqxx::work txn(conn);
    txn.exec_params(
            "UPDATE portfolio_holdings SET quantity = \$1 WHERE holding_id = \$2",
            portfolioHolding.getQuantity(), portfolioHolding.getId()
    );
    txn.commit();
}

void PortfolioHoldingDao::deletePortfolioHolding(int id) {
    pqxx::work txn(conn);
    txn.exec_params("DELETE FROM portfolio_holdings WHERE holding_id = \$1", id);
    txn.commit();
}
