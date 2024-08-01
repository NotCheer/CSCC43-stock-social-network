//
// Created by xinyuli on 8/1/24.
//

#ifndef CSCC43_STOCK_SOCIAL_NETWORK_STOCK_H
#define CSCC43_STOCK_SOCIAL_NETWORK_STOCK_H


#include <string>

class Stock {
private:
    int id;
    std::string symbol;
    std::string companyName;
    double currentPrice;

public:
    Stock(int id, const std::string& symbol, const std::string& companyName, double currentPrice);

    int getId() const;
    void setId(int id);

    std::string getSymbol() const;
    void setSymbol(const std::string& symbol);

    std::string getCompanyName() const;
    void setCompanyName(const std::string& companyName);

    double getCurrentPrice() const;
    void setCurrentPrice(double currentPrice);
};


#endif //CSCC43_STOCK_SOCIAL_NETWORK_STOCK_H
