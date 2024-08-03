#include "StockHistory.h"

StockHistory::StockHistory(int stockHistoryId, const std::string& timestamp, double open, double high, double low, double close, long volume, const std::string& symbol)
        : stockHistoryId(stockHistoryId), timestamp(timestamp), open(open), high(high), low(low), close(close), volume(volume), symbol(symbol) {}

int StockHistory::getStockHistoryId() const {
    return stockHistoryId;
}

void StockHistory::setStockHistoryId(int stockHistoryId) {
    this->stockHistoryId = stockHistoryId;
}

std::string StockHistory::getTimestamp() const {
    return timestamp;
}

void StockHistory::setTimestamp(const std::string& timestamp) {
    this->timestamp = timestamp;
}

double StockHistory::getOpen() const {
    return open;
}

void StockHistory::setOpen(double open) {
    this->open = open;
}

double StockHistory::getHigh() const {
    return high;
}

void StockHistory::setHigh(double high) {
    this->high = high;
}

double StockHistory::getLow() const {
    return low;
}

void StockHistory::setLow(double low) {
    this->low = low;
}

double StockHistory::getClose() const {
    return close;
}

void StockHistory::setClose(double close) {
    this->close = close;
}

long StockHistory::getVolume() const {
    return volume;
}

void StockHistory::setVolume(long volume) {
    this->volume = volume;
}

std::string StockHistory::getSymbol() const {
    return symbol;
}

void StockHistory::setSymbol(const std::string& symbol) {
    this->symbol = symbol;
}
