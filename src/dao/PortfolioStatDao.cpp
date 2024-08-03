#include "PortfolioStatDao.h"
#include <iostream>

PortfolioStatDao::PortfolioStatDao(const std::string& conn_str) : connection_string(conn_str) {}

int PortfolioStatDao::addPortfolioStat(const PortfolioStat& portfolio_stat) {
    try {
        pqxx::connection C(connection_string);
        pqxx::work W(C);
        std::string sql = "INSERT INTO portfolio_stats (portfolio_id, cov, beta, cov_matrix) VALUES (" +
                          W.quote(portfolio_stat.getPortfolioId()) + ", " +
                          W.quote(portfolio_stat.getCov()) + ", " +
                          W.quote(portfolio_stat.getBeta()) + ", " +
                          W.quote(portfolio_stat.getCovMatrix()) + ") RETURNING stat_id";
        pqxx::result R = W.exec(sql);
        W.commit();
        return R[0][0].as<int>();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        throw;
    }
}

PortfolioStat PortfolioStatDao::getPortfolioStatById(int stat_id) {
    try {
        pqxx::connection C(connection_string);
        pqxx::nontransaction N(C);
        std::string sql = "SELECT stat_id, portfolio_id, cov, beta, cov_matrix FROM portfolio_stats WHERE stat_id = " + N.quote(stat_id);
        pqxx::result R(N.exec(sql));

        if (R.size() != 1) {
            throw std::runtime_error("PortfolioStat not found");
        }

        pqxx::row row = R[0];
        return PortfolioStat(row["stat_id"].as<int>(), row["portfolio_id"].as<int>(), row["cov"].as<double>(), row["beta"].as<double>(), row["cov_matrix"].as<std::string>());
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        throw;
    }
}

std::vector<PortfolioStat> PortfolioStatDao::getPortfolioStatsByPortfolioId(int portfolio_id) {
    std::vector<PortfolioStat> portfolio_stats;
    try {
        pqxx::connection C(connection_string);
        pqxx::nontransaction N(C);
        std::string sql = "SELECT stat_id, portfolio_id, cov, beta, cov_matrix FROM portfolio_stats WHERE portfolio_id = " + N.quote(portfolio_id);
        pqxx::result R(N.exec(sql));

        for (auto row : R) {
            portfolio_stats.emplace_back(row["stat_id"].as<int>(), row["portfolio_id"].as<int>(), row["cov"].as<double>(), row["beta"].as<double>(), row["cov_matrix"].as<std::string>());
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return portfolio_stats;
}

void PortfolioStatDao::updatePortfolioStat(const PortfolioStat& portfolio_stat) {
    try {
        pqxx::connection C(connection_string);
        pqxx::work W(C);
        std::string sql = "UPDATE portfolio_stats SET portfolio_id = " + W.quote(portfolio_stat.getPortfolioId()) +
                          ", cov = " + W.quote(portfolio_stat.getCov()) +
                          ", beta = " + W.quote(portfolio_stat.getBeta()) +
                          ", cov_matrix = " + W.quote(portfolio_stat.getCovMatrix()) +
                          " WHERE stat_id = " + W.quote(portfolio_stat.getStatId());
        W.exec(sql);
        W.commit();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        throw;
    }
}

void PortfolioStatDao::deletePortfolioStat(int stat_id) {
    try {
        pqxx::connection C(connection_string);
        pqxx::work W(C);
        std::string sql = "DELETE FROM portfolio_stats WHERE stat_id = " + W.quote(stat_id);
        W.exec(sql);
        W.commit();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        throw;
    }
}
