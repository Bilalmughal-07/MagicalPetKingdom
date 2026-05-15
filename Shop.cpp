//Muhammad Bilal
//24I-3168
//SE-A

#include "Shop.h"
#include "Item.h"
#include <iostream>
#include <cstdlib>

Shop::Shop() : itemCount(0) {
    for (int i = 0; i < MAX_ITEMS; i++) items[i] = nullptr;
    restock();
}

Shop::~Shop() {
    for (int i = 0; i < itemCount; i++) {
        delete items[i];
    }
}

void Shop::displayItems() const {
    std::cout << "===== SHOP =====" << std::endl;
    std::cout << "Available Items:\n";
    for (int i = 0; i < itemCount; i++) {
        std::cout << i + 1 << ". " << items[i]->getName()
            << " - " << items[i]->getDescription()
            << " (" << items[i]->getPrice() << " coins)"
            << " [x" << items[i]->getQuantity() << "]\n";
    }
    std::cout << "=================" << std::endl;
}

Item* Shop::purchaseItem(int index, int& playerCoins) {
    if (index < 0 || index >= itemCount) {
        std::cout << "Invalid item selection!\n";
        return nullptr;
    }

    Item* selected = items[index];
    if (playerCoins < selected->getPrice()) {
        std::cout << "Not enough coins!\n";
        return nullptr;
    }

    playerCoins -= selected->getPrice();
    Item* purchased = selected->clone();
    purchased->setQuantity(1);

    // Remove item from shop if it was the last one
    if (selected->getQuantity() <= 1) {
        delete items[index];
        for (int i = index; i < itemCount - 1; i++) {
            items[i] = items[i + 1];
        }
        items[itemCount - 1] = nullptr;
        itemCount--;
    }
    else {
        selected->setQuantity(selected->getQuantity() - 1);
    }

    return purchased;
}

bool Shop::addItem(Item* item) {
    if (itemCount >= MAX_ITEMS) {
        return false;
    }

    // Check if item already exists
    for (int i = 0; i < itemCount; i++) {
        if (items[i]->getName() == item->getName()) {
            items[i]->setQuantity(items[i]->getQuantity() + item->getQuantity());
            delete item;
            return true;
        }
    }

    // Add new item
    items[itemCount++] = item;
    return true;
}

void Shop::restock() {
    // Clear existing items
    for (int i = 0; i < itemCount; i++) {
        delete items[i];
        items[i] = nullptr;
    }
    itemCount = 0;

    // Add new items
    items[itemCount++] = new HealingPotion();
    items[itemCount++] = new ManaPotion();
    items[itemCount++] = new SpeedBoost();
    items[itemCount++] = new AttackBoost();
    items[itemCount++] = new DefenseShield();

    // Set quantities (3-5 each)
    for (int i = 0; i < itemCount; i++) {
        items[i]->setQuantity(3 + (rand() % 3));
    }
}

int Shop::getItemCount() const {
    return itemCount;
}

Item* Shop::getItem(int index) const {
    if (index < 0 || index >= itemCount) return nullptr;
    return items[index];
}