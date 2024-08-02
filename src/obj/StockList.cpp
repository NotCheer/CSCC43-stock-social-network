#include "StockList.h"

StockList::StockList(int userId, const std::string& name, bool isPublic)
        : userId(userId), name(name), isPublic(isPublic) {}

int StockList::getListId() const {
    return listId;
}

void StockList::setListId(int listId) {
    this->listId = listId;
}

int StockList::getUserId() const {
    return userId;
}

void StockList::setUserId(int userId) {
    this->userId = userId;
}

std::string StockList::getName() const {
    return name;
}

void StockList::setName(const std::string& name) {
    this->name = name;
}

bool StockList::getIsPublic() const {
    return isPublic;
}

void StockList::setIsPublic(bool isPublic) {
    this->isPublic = isPublic;
}
