//Muhammad Bilal
//24I-3168
//SE-A

#ifndef SHOP_H
#define SHOP_H

#include "Item.h"

class Shop {
private:
    static const int MAX_ITEMS = 10;
    Item* items[MAX_ITEMS];
    int itemCount;

public:
    Shop();
    ~Shop();

    void displayItems() const;
    bool addItem(Item* item);
    Item* purchaseItem(int index, int& playerCoins);
    void restock();
    int getItemCount() const;
    Item* getItem(int index) const;
};

#endif // SHOP_H
