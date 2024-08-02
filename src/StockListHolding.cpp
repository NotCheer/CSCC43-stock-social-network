#include "StockListHolding.h"

StockListHolding::StockListHolding(int list_id, int stock_id, int shares)
        : list_id(list_id), stock_id(stock_id), shares(shares) {}

StockListHolding::StockListHolding(int list_holding_id, int list_id, int stock_id, int shares)
        : list_holding_id(list_holding_id), list_id(list_id), stock_id(stock_id), shares(shares) {}

int StockListHolding::getListHoldingId() const {
    return list_holding_id;
}

int StockListHolding::getListId() const {
    return list_id;
}

int StockListHolding::getStockId() const {
    return stock_id;
}

int StockListHolding::getShares() const {
    return shares;
}

void StockListHolding::setListId(int list_id) {
    this->list_id = list_id;
}

void StockListHolding::setStockId(int stock_id) {
    this->stock_id = stock_id;
}

void StockListHolding::setShares(int shares) {
    this->shares = shares;
}
