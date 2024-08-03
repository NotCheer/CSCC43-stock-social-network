#include "Stock.h"

Stock::Stock(int id, const std::string& symbol, const std::string& companyName, double currentPrice)
        : id(id), symbol(symbol), companyName(companyName), currentPrice(currentPrice) {}

int Stock::getId() const {
    return id;
}

void Stock::setId(int id) {
    this->id = id;
}

std::string Stock::getSymbol() const {
    return symbol;
}

void Stock::setSymbol(const std::string& symbol) {
    this->symbol = symbol;
}

std::string Stock::getCompanyName() const {
    return companyName;
}

void Stock::setCompanyName(const std::string& companyName) {
    this->companyName = companyName;
}

double Stock::getCurrentPrice() const {
    return currentPrice;
}

void Stock::setCurrentPrice(double currentPrice) {
    this->currentPrice = currentPrice;
}
