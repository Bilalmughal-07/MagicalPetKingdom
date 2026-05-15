//Muhammad Bilal
//24I-3168
//SE-A

#ifndef ITEM_H
#define ITEM_H

#include <string>

class Pet; // Forward declaration

class Item {
protected:
    std::string name;
    std::string type;
    std::string description;
    int price;
    int quantity;

public:
    Item(const std::string& name, const std::string& description, int price, const std::string& type);
    virtual ~Item() {}

    // Getters
    std::string getName() const;
    std::string getDescription() const;
    std::string getType() const;
    int getPrice() const;
    int getQuantity() const;

    // Setters
    void setQuantity(int qty);

    // Usage
    virtual void use(Pet* target) = 0;
    virtual Item* clone() const = 0;
};

class HealingPotion : public Item {
private:
    int healAmount;

public:
    HealingPotion();
    void use(Pet* target) override;
    Item* clone() const override;
};

class ManaPotion : public Item {
public:
    ManaPotion();
    void use(Pet* target) override;
    Item* clone() const override;
};

class SpeedBoost : public Item {
private:
    float boostAmount;

public:
    SpeedBoost();
    void use(Pet* target) override;
    Item* clone() const override;
};

class AttackBoost : public Item {
private:
    int boostAmount;

public:
    AttackBoost();
    void use(Pet* target) override;
    Item* clone() const override;
};

class DefenseShield : public Item {
private:
    int shieldAmount;

public:
    DefenseShield();
    void use(Pet* target) override;
    Item* clone() const override;
};

#endif // ITEM_H
