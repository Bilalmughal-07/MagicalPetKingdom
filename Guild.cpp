//Muhammad Bilal
//24I-3168
//SE-A

#include "Guild.h"
#include <iostream>
using namespace std;

Guild::Guild() : name(""), petCount(0), wins(0), losses(0) {
    for (int i = 0; i < MAX_PETS; i++) {
        petNames[i] = "";
        petRoles[i] = ROLE_DAMAGE_DEALER;
    }
}

Guild::Guild(const string& guildName) : name(guildName), petCount(0), wins(0), losses(0) {
    for (int i = 0; i < MAX_PETS; i++) {
        petNames[i] = "";
        petRoles[i] = ROLE_DAMAGE_DEALER;
    }
}

bool Guild::addPet(const string& petName, PetRole role) {
    if (petCount >= MAX_PETS) return false;
    // Check for duplicates
    for (int i = 0; i < petCount; i++) {
        if (petNames[i] == petName) return false;
    }
    petNames[petCount] = petName;
    petRoles[petCount] = role;
    petCount++;
    return true;
}

bool Guild::removePet(int index) {
    if (index < 0 || index >= petCount) return false;
    for (int i = index; i < petCount - 1; i++) {
        petNames[i] = petNames[i + 1];
        petRoles[i] = petRoles[i + 1];
    }
    petCount--;
    return true;
}

void Guild::setPetRole(int index, PetRole role) {
    if (index >= 0 && index < petCount) petRoles[index] = role;
}

string Guild::getName() const { return name; }
int Guild::getPetCount() const { return petCount; }
string Guild::getPetName(int index) const {
    if (index >= 0 && index < petCount) return petNames[index];
    return "";
}
PetRole Guild::getPetRole(int index) const {
    if (index >= 0 && index < petCount) return petRoles[index];
    return ROLE_DAMAGE_DEALER;
}
string Guild::getRoleName(PetRole role) const {
    switch (role) {
    case ROLE_TANK: return "Tank";
    case ROLE_HEALER: return "Healer";
    case ROLE_DAMAGE_DEALER: return "Damage Dealer";
    default: return "Unknown";
    }
}
int Guild::getWins() const { return wins; }
int Guild::getLosses() const { return losses; }
void Guild::addWin() { wins++; }
void Guild::addLoss() { losses++; }
void Guild::setName(const string& n) { name = n; }
void Guild::setWins(int w) { wins = w; }
void Guild::setLosses(int l) { losses = l; }
