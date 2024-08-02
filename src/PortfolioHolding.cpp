#include "PortfolioHolding.h"

PortfolioHolding::PortfolioHolding(int portfolioId, int stockId, double quantity)
        : portfolioId(portfolioId), stockId(stockId), quantity(quantity) {}

int PortfolioHolding::getId() const { return id; }
int PortfolioHolding::getPortfolioId() const { return portfolioId; }
int PortfolioHolding::getStockId() const { return stockId; }
double PortfolioHolding::getQuantity() const { return quantity; }

void PortfolioHolding::setId(int id) { this->id = id; }
void PortfolioHolding::setQuantity(double quantity) { this->quantity = quantity; }
