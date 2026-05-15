//Muhammad Bilal
//24I-3168
//SE-A

#include "Item.h"
#include "Pet.h"
#include <iostream>
using namespace std;

// Base Item implementation
Item::Item(const std::string& name, const std::string& description, int price, const std::string& type)
{
    this->name = name;
    this->description = description;
    this->price = price;
    this->type = type;
    quantity = 1;
}

std::string Item::getName() const { return name; }
std::string Item::getDescription() const { return description; }
std::string Item::getType() const { return type; }
int Item::getPrice() const { return price; }
int Item::getQuantity() const { return quantity; }
void Item::setQuantity(int qty) { quantity = qty; }

// Healing Potion implementation
HealingPotion::HealingPotion()
    : Item("Healing Potion", "Restores 50 HP to a pet", 20, "heal"), healAmount(50) {
}

void HealingPotion::use(Pet* target) {
    if (target && quantity > 0) {
        target->heal(healAmount);
        quantity--;
        cout << "Used Healing Potion on " << target->getName()
            << ". Restored " << healAmount << " HP." << endl;
    }
}

Item* HealingPotion::clone() const {
    return new HealingPotion(*this);
}

// Mana Potion implementation
ManaPotion::ManaPotion()
    : Item("Mana Potion", "Resets all ability cooldowns", 30, "mana") {
}

void ManaPotion::use(Pet* target) {
    if (target && quantity > 0) {
        target->resetCooldowns();
        quantity--;
        cout << "Used Mana Potion on " << target->getName()
            << ". All abilities refreshed!" << endl;
    }
}

Item* ManaPotion::clone() const {
    return new ManaPotion(*this);
}

// Speed Boost implementation
SpeedBoost::SpeedBoost()
    : Item("Speed Boost", "Increases speed by 0.5", 25, "buff"),
    boostAmount(0.5f) {
}

void SpeedBoost::use(Pet* target) {
    if (target && quantity > 0) {
        float currentSpeed = target->getSpeed();
        target->setSpeed(currentSpeed + boostAmount);
        quantity--;
        cout << "Used Speed Boost on " << target->getName()
            << ". Speed increased by " << boostAmount << "!" << endl;
    }
}

Item* SpeedBoost::clone() const {
    return new SpeedBoost(*this);
}

// Attack Boost implementation
AttackBoost::AttackBoost()
    : Item("Attack Boost", "Increases attack by 10", 30, "buff"),
    boostAmount(10) {
}

void AttackBoost::use(Pet* target) {
    if (target && quantity > 0) {
        int currentAttack = target->getAttack();
        target->setAttack(currentAttack + boostAmount);
        quantity--;
        cout << "Used Attack Boost on " << target->getName()
            << ". Attack increased by " << boostAmount << "!" << endl;
    }
}

Item* AttackBoost::clone() const {
    return new AttackBoost(*this);
}

// Defense Shield implementation
DefenseShield::DefenseShield()
    : Item("Defense Shield", "Increases defense by 15", 35, "shield"),
    shieldAmount(15) {
}

void DefenseShield::use(Pet* target) {
    if (target && quantity > 0) {
        int currentDefense = target->getDefense();
        target->setDefense(currentDefense + shieldAmount);
        quantity--;
        cout << "Used Defense Shield on " << target->getName()
            << ". Defense increased by " << shieldAmount << "!" << endl;
    }
}

Item* DefenseShield::clone() const {
    return new DefenseShield(*this);
}