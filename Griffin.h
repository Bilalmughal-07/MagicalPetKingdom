//Muhammad Bilal
//24I-3168
//SE-A
#ifndef GRIFFIN_H
#define GRIFFIN_H

#include "Pet.h"

class Griffin : public Pet {
private:
    bool clawStrikeAvailable;
    bool diveBombAvailable;
    
    Clock diveBombCooldownClock;
    float diveBombCooldownTime;

public:
    Griffin(const string& name);

   
    // Overridden virtual functions
    void useAbility1(Pet* target) override;
    void useAbility2(Pet* target, Pet* target2=nullptr) override;
    void displayStats() const override;
    void updateCoolDown() override;
    void resetCooldowns() override;

    // Griffin-specific abilities
    void clawStrike(Pet* target);
    void diveBomb(Pet*  target,Pet* target2);

    // Status checkers
    bool canUseClawStrike() const;
    bool canUseDiveBomb() const;

    // Cooldown/reset methods
    void resetAbilities();
};

#endif
