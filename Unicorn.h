//Muhammad Bilal
//24I-3168
//SE-A

#ifndef UNICORN_H
#define UNICORN_H

#include "Pet.h"
//unicrorn header file
class Unicorn : public Pet {
private:
    //private functoina and vaiable
    bool speedBurstAvailable;
    bool magicShieldAvailable;
    bool speedBoostActive;
    Clock speedBoostclock;

    bool shieldActive = false;
    // to track real time in secind
    Clock shieldTimer, shieldCooldownClock;
    float shieldDuration = 5.0f, shieldCooldownTime=10.0f,speedBoostduration=5.0f;


public:
    //constructor
    Unicorn(const string& name);

  

    bool isShieldActive() const;
    // Overridden virtual functions
    void useAbility1(Pet* target) override;
    void useAbility2(Pet* target, Pet* target2 = nullptr) override;
    void updateCoolDown() override ;
    void resetCooldowns() override;
    void displayStats() const override;
    // Unicorn-specific abilities
    void activateSpeedBurst();
    void activateMagicShield();
    void deactivateShield();

    // Status checkers
   /* bool canUseSpeedBurst() const;*/

    // Combat methods
    void takeDamage(int damage) override;

   
};

#endif#pragma once
