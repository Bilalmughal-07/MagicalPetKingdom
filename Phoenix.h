//Muhammad Bilal
//24I-3168
//SE-A


#ifndef PHOENIX_H
#define PHOENIX_H

#include "Pet.h"
#include <SFML/Graphics.hpp>
class Phoenix :public Pet{
private:
    bool flameHealingAvailable;
    bool rebirthAvailable=true;
    bool hasRebirthed=false;
    Pet* activeHealingTarget;
    int healingHpPerSecond;
    int healingDuration;
    Clock healingTimer;
    int lastHealTime;
    float totalHealingTime;
    bool isHealingActive;

public:
    Phoenix(const string& name);
    void startHealingOverTime(Pet* target, int hpPerSecond, int durationSeconds);
    void showHealingSelectionWindow(Pet* target);
    void updateHealingOverTime();

    void useAbility1(Pet* target) override;
    void useAbility2(Pet* target, Pet* target2 = nullptr) override;
    void updateCoolDown() override;
    void resetCooldowns() override;
    void flameHealing(Pet* target, RenderWindow& window);
    bool rebirth();
    bool canUseFlameHealing() const;
    bool canRebirth() const;
    bool hasUsedRebirth() const;
    void displayStats() const override;
    void takeDamage(int damage) override;
};

#endif#
