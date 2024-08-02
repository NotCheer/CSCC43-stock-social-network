//
// Created by xinyuli on 8/1/24.
//

#ifndef CSCC43_STOCK_SOCIAL_NETWORK_PORTFOLIO_H
#define CSCC43_STOCK_SOCIAL_NETWORK_PORTFOLIO_H


#include <string>

class Portfolio {
private:
    int id;
    int userId;
    std::string name;
    double cashAccount;

public:
    Portfolio(int userId, const std::string& name, double cashAccount);

    int getId() const;
    int getUserId() const;
    std::string getName() const;
    double getCashAccount() const;

    void setId(int id);
    void setName(const std::string& name);
    void setCashAccount(double cashAccount);
};


#endif //CSCC43_STOCK_SOCIAL_NETWORK_PORTFOLIO_H
