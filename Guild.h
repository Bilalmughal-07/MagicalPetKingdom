//Muhammad Bilal
//24I-3168
//SE-A

#ifndef GUILD_H
#define GUILD_H

#include <string>
#include "Pet.h"

// Pet roles in a guild
enum PetRole {
    ROLE_TANK,
    ROLE_HEALER,
    ROLE_DAMAGE_DEALER,
    ROLE_COUNT
};

class Guild {
private:
    std::string name;
    static const int MAX_PETS = 4;
    std::string petNames[MAX_PETS];
    PetRole petRoles[MAX_PETS];
    int petCount;
    int wins;
    int losses;

public:
    Guild();
    Guild(const std::string& guildName);

    // Pet management
    bool addPet(const std::string& petName, PetRole role);
    bool removePet(int index);
    void setPetRole(int index, PetRole role);

    // Getters
    std::string getName() const;
    int getPetCount() const;
    std::string getPetName(int index) const;
    PetRole getPetRole(int index) const;
    std::string getRoleName(PetRole role) const;
    int getWins() const;
    int getLosses() const;

    // Stats
    void addWin();
    void addLoss();
    void setName(const std::string& n);
    void setWins(int w);
    void setLosses(int l);
};

#endif // GUILD_H
