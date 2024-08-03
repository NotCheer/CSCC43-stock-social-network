//
// Created by xinyuli on 8/3/24.
//

#ifndef CSCC43_STOCK_SOCIAL_NETWORK_STOCKHISTORY_H
#define CSCC43_STOCK_SOCIAL_NETWORK_STOCKHISTORY_H


#include <string>
#include <ctime>

class StockHistory {
public:
    StockHistory() = default;
    StockHistory(int stockHistoryId, const std::string& timestamp, double open, double high, double low, double close, long volume, const std::string& symbol);

    // Getters and Setters
    int getStockHistoryId() const;
    void setStockHistoryId(int stockHistoryId);

    std::string getTimestamp() const;
    void setTimestamp(const std::string& timestamp);

    double getOpen() const;
    void setOpen(double open);

    double getHigh() const;
    void setHigh(double high);

    double getLow() const;
    void setLow(double low);

    double getClose() const;
    void setClose(double close);

    long getVolume() const;
    void setVolume(long volume);

    std::string getSymbol() const;
    void setSymbol(const std::string& symbol);

private:
    int stockHistoryId;
    std::string timestamp;
    double open;
    double high;
    double low;
    double close;
    long volume;
    std::string symbol;
};


#endif //CSCC43_STOCK_SOCIAL_NETWORK_STOCKHISTORY_H
