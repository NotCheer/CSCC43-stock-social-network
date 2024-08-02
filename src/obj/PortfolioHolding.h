//
// Created by xinyuli on 8/1/24.
//

#ifndef CSCC43_STOCK_SOCIAL_NETWORK_PORTFOLIOHOLDING_H
#define CSCC43_STOCK_SOCIAL_NETWORK_PORTFOLIOHOLDING_H


class PortfolioHolding {
private:
    int id;
    int portfolioId;
    int stockId;
    double quantity;

public:
    PortfolioHolding(int portfolioId, int stockId, double quantity);

    int getId() const;
    int getPortfolioId() const;
    int getStockId() const;
    double getQuantity() const;

    void setId(int id);
    void setQuantity(double quantity);
};


#endif //CSCC43_STOCK_SOCIAL_NETWORK_PORTFOLIOHOLDING_H
