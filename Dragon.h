//Muhammad Bilal
//24I-3168
//SE-A



#ifndef DRAGON_H
#define DRAGON_H

#include "Pet.h"
#include <SFML/System/Clock.hpp>
using namespace sf;
class Dragon :public Pet{
private:
 // private varaible 
    bool fireballAvailable;
    bool roarAvailable;
    Clock roarCooldownClock; // clock
    float RoarCooldownTime = 10.0f;//cooldown time
    

public:
    Dragon(const string& name="Dragon");

    // virtual funciotn for ability 1 and 2
    void useAbility1(Pet* target) override;
    void useAbility2(Pet* target, Pet* target2 = nullptr) override;
    //other function
    void updateCoolDown() override ;
    void resetCooldowns() override;
    void fireballAttack(Pet* target);
    void dragonsRoar(Pet* target1,Pet* target2=nullptr);
   

    void displayStats() const override;
};

#endif