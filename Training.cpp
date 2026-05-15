//Muhammad Bilal
//24I-3168
//SE-A

#include "Training.h"
#include <iostream>
using namespace std;

TrainingCamp::TrainingCamp()
    : hpGain(15), attackGain(5), speedGain(0.2f), defenseGain(4) {
}

bool TrainingCamp::trainPet(Pet* pet, TrainingType type, int& coins) {
    if (!pet) return false;
    if (coins < TRAINING_COST) {
        cout << "Not enough coins for training! Need " << TRAINING_COST << " coins." << endl;
        return false;
    }

    coins -= TRAINING_COST;

    switch (type) {
    case TRAINING_HP:
        pet->setMaxHp(pet->getMaxHp() + hpGain);
        pet->setHp(pet->getMaxHp()); // Fully heal after HP training
        cout << pet->getName() << " completed Obstacle Course! Max HP +" << hpGain << endl;
        break;
    case TRAINING_ATTACK:
        pet->setAttack(pet->getAttack() + attackGain);
        cout << pet->getName() << " completed Combat Drill! Attack +" << attackGain << endl;
        break;
    case TRAINING_SPEED:
        pet->setSpeed(pet->getSpeed() + speedGain);
        cout << pet->getName() << " completed Agility Run! Speed +" << speedGain << endl;
        break;
    case TRAINING_DEFENSE:
        pet->setDefense(pet->getDefense() + defenseGain);
        cout << pet->getName() << " completed Shield Practice! Defense +" << defenseGain << endl;
        break;
    default:
        coins += TRAINING_COST; // Refund
        return false;
    }

    // Award some EXP for training
    pet->gainExp(25);
    return true;
}

int TrainingCamp::getCost() const { return TRAINING_COST; }

string TrainingCamp::getTrainingName(TrainingType type) const {
    switch (type) {
    case TRAINING_HP:      return "Obstacle Course";
    case TRAINING_ATTACK:  return "Combat Drill";
    case TRAINING_SPEED:   return "Agility Run";
    case TRAINING_DEFENSE: return "Shield Practice";
    default: return "Unknown";
    }
}

string TrainingCamp::getTrainingDescription(TrainingType type) const {
    switch (type) {
    case TRAINING_HP:      return "Train endurance to increase max HP";
    case TRAINING_ATTACK:  return "Practice combat techniques to boost attack";
    case TRAINING_SPEED:   return "Run obstacle courses to improve speed";
    case TRAINING_DEFENSE: return "Block and dodge to strengthen defense";
    default: return "";
    }
}

string TrainingCamp::getStatGain(TrainingType type) const {
    switch (type) {
    case TRAINING_HP:      return "HP +" + to_string(hpGain);
    case TRAINING_ATTACK:  return "ATK +" + to_string(attackGain);
    case TRAINING_SPEED:   return "SPD +0.2";
    case TRAINING_DEFENSE: return "DEF +" + to_string(defenseGain);
    default: return "";
    }
}
