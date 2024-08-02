//
// Created by xinyuli on 8/2/24.
//

#ifndef CSCC43_STOCK_SOCIAL_NETWORK_STOCKLIST_H
#define CSCC43_STOCK_SOCIAL_NETWORK_STOCKLIST_H


#include <string>

class StockList {
private:
    int listId;
    int userId;
    std::string name;
    bool isPublic;

public:
    StockList() = default;
    StockList(int userId, const std::string& name, bool isPublic);

    int getListId() const;
    void setListId(int listId);

    int getUserId() const;
    void setUserId(int userId);

    std::string getName() const;
    void setName(const std::string& name);

    bool getIsPublic() const;
    void setIsPublic(bool isPublic);
};


#endif //CSCC43_STOCK_SOCIAL_NETWORK_STOCKLIST_H
