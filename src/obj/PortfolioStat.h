//
// Created by xinyuli on 8/2/24.
//

#ifndef CSCC43_STOCK_SOCIAL_NETWORK_PORTFOLIOSTAT_H
#define CSCC43_STOCK_SOCIAL_NETWORK_PORTFOLIOSTAT_H


#include <string>

class PortfolioStat {
private:
    int stat_id;
    int portfolio_id;
    double cov;
    double beta;
    std::string cov_matrix;

public:
    PortfolioStat(int portfolio_id, double cov, double beta, const std::string& cov_matrix);
    PortfolioStat(int stat_id, int portfolio_id, double cov, double beta, const std::string& cov_matrix);

    int getStatId() const;
    int getPortfolioId() const;
    double getCov() const;
    double getBeta() const;
    std::string getCovMatrix() const;

    void setPortfolioId(int portfolio_id);
    void setCov(double cov);
    void setBeta(double beta);
    void setCovMatrix(const std::string& cov_matrix);
};



#endif //CSCC43_STOCK_SOCIAL_NETWORK_PORTFOLIOSTAT_H
