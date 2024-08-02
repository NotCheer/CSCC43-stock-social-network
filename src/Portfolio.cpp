#include "Portfolio.h"

Portfolio::Portfolio(int userId, const std::string& name, double cashAccount)
        : userId(userId), name(name), cashAccount(cashAccount) {}

int Portfolio::getId() const { return id; }
int Portfolio::getUserId() const { return userId; }
std::string Portfolio::getName() const { return name; }
double Portfolio::getCashAccount() const { return cashAccount; }

void Portfolio::setId(int id) { this->id = id; }
void Portfolio::setName(const std::string& name) { this->name = name; }
void Portfolio::setCashAccount(double cashAccount) { this->cashAccount = cashAccount; }
