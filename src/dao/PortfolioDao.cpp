#include "PortfolioDao.h"

PortfolioDao::PortfolioDao(pqxx::connection& conn) : conn(conn) {}

void PortfolioDao::createPortfolio(Portfolio& portfolio) {
    pqxx::work txn(conn);
    pqxx::result result = txn.exec_params(
            "INSERT INTO portfolios (user_id, name, cash_account) VALUES (\$1, \$2, \$3) RETURNING portfolio_id",
            portfolio.getUserId(), portfolio.getName(), portfolio.getCashAccount()
    );
    portfolio.setId(result[0][0].as<int>());
    txn.commit();
}

Portfolio PortfolioDao::getPortfolioById(int id) {
    pqxx::work txn(conn);
    pqxx::result result = txn.exec_params("SELECT * FROM portfolios WHERE portfolio_id = \$1", id);

    if (result.empty()) {
        throw std::runtime_error("Portfolio not found");
    }

    const auto& row = result[0];
    Portfolio portfolio(row["user_id"].as<int>(), row["name"].as<std::string>(), row["cash_account"].as<double>());
    portfolio.setId(row["portfolio_id"].as<int>());
    return portfolio;
}

std::vector<Portfolio> PortfolioDao::getPortfoliosByUserId(int userId) {
    pqxx::work txn(conn);
    pqxx::result result = txn.exec_params("SELECT * FROM portfolios WHERE user_id = \$1", userId);

    std::vector<Portfolio> portfolios;
    for (const auto& row : result) {
        Portfolio portfolio(row["user_id"].as<int>(), row["name"].as<std::string>(), row["cash_account"].as<double>());
        portfolio.setId(row["portfolio_id"].as<int>());
        portfolios.push_back(portfolio);
    }

    return portfolios;
}

void PortfolioDao::updatePortfolio(const Portfolio& portfolio) {
    pqxx::work txn(conn);
    txn.exec_params(
            "UPDATE portfolios SET name = \$1, cash_account = \$2 WHERE portfolio_id = \$3",
            portfolio.getName(), portfolio.getCashAccount(), portfolio.getId()
    );
    txn.commit();
}

void PortfolioDao::deletePortfolio(int id) {
    pqxx::work txn(conn);
    txn.exec_params("DELETE FROM portfolios WHERE portfolio_id = \$1", id);
    txn.commit();
}
