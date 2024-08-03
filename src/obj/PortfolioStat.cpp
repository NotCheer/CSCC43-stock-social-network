#include "PortfolioStat.h"

PortfolioStat::PortfolioStat(int portfolio_id, double cov, double beta, const std::string& cov_matrix)
        : portfolio_id(portfolio_id), cov(cov), beta(beta), cov_matrix(cov_matrix) {}

PortfolioStat::PortfolioStat(int stat_id, int portfolio_id, double cov, double beta, const std::string& cov_matrix)
        : stat_id(stat_id), portfolio_id(portfolio_id), cov(cov), beta(beta), cov_matrix(cov_matrix) {}

int PortfolioStat::getStatId() const {
    return stat_id;
}

int PortfolioStat::getPortfolioId() const {
    return portfolio_id;
}

double PortfolioStat::getCov() const {
    return cov;
}

double PortfolioStat::getBeta() const {
    return beta;
}

std::string PortfolioStat::getCovMatrix() const {
    return cov_matrix;
}

void PortfolioStat::setPortfolioId(int portfolio_id) {
    this->portfolio_id = portfolio_id;
}

void PortfolioStat::setCov(double cov) {
    this->cov = cov;
}

void PortfolioStat::setBeta(double beta) {
    this->beta = beta;
}

void PortfolioStat::setCovMatrix(const std::string& cov_matrix) {
    this->cov_matrix = cov_matrix;
}
