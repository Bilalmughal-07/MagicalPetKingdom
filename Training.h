//Muhammad Bilal
//24I-3168
//SE-A

#ifndef TRAINING_H
#define TRAINING_H

#include <string>
#include "Pet.h"

// Training types available at Training Camps
enum TrainingType {
    TRAINING_HP,        // Obstacle Course - increases HP
    TRAINING_ATTACK,    // Combat Drill - increases Attack
    TRAINING_SPEED,     // Agility Run - increases Speed
    TRAINING_DEFENSE,   // Shield Practice - increases Defense
    TRAINING_COUNT
};

// Training Camp class - manages pet training sessions
class TrainingCamp {
private:
    static const int TRAINING_COST = 30;           // Coins per session
    static const int TRAINING_POINTS_PER_SESSION = 1;

    // Stat gains per training type
    int hpGain;
    int attackGain;
    float speedGain;
    int defenseGain;

public:
    TrainingCamp();

    // Train a pet in a specific discipline
    bool trainPet(Pet* pet, TrainingType type, int& coins);

    // Getters for display
    int getCost() const;
    std::string getTrainingName(TrainingType type) const;
    std::string getTrainingDescription(TrainingType type) const;
    std::string getStatGain(TrainingType type) const;
};

#endif // TRAINING_H
