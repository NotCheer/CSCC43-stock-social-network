//
// Created by xinyuli on 8/2/24.
//

#ifndef CSCC43_STOCK_SOCIAL_NETWORK_STOCKLISTHOLDING_H
#define CSCC43_STOCK_SOCIAL_NETWORK_STOCKLISTHOLDING_H


class StockListHolding {
private:
    int list_holding_id;
    int list_id;
    int stock_id;
    int shares;

public:
    StockListHolding(int list_id, int stock_id, int shares);
    StockListHolding(int list_holding_id, int list_id, int stock_id, int shares);

    int getListHoldingId() const;
    int getListId() const;
    int getStockId() const;
    int getShares() const;

    void setListId(int list_id);
    void setStockId(int stock_id);
    void setShares(int shares);
};


#endif //CSCC43_STOCK_SOCIAL_NETWORK_STOCKLISTHOLDING_H
