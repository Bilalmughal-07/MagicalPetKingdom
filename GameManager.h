//Muhammad Bilal
//24I-3168
//SE-A

#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <SFML/Graphics.hpp>
#include <string>
#include "LeaderBoard.h"
#include "Item.h"
#include "Shop.h"
#include "Training.h"
#include "Guild.h"
#include <iostream>
using namespace std;
using namespace sf;

// Stores persistent bonus stats earned through training
struct PetBonusStats {
    int bonusHp = 0;
    int bonusAttack = 0;
    float bonusSpeed = 0.0f;
    int bonusDefense = 0;
    int level = 1;
    int exp = 0;
};

class GameManager {
private:
    static GameManager* instance;

    // Window and rendering
    RenderWindow* window;
    Font font;
    LeaderBoard lb;
    
    // Game state
    bool gameRunning;
    bool nameEntered;
    void exitGame(RenderWindow* window);
    // Player info
    string playerName;

    // Economy and Inventory
    static const int MAX_INVENTORY = 20;
    Item* inventory[MAX_INVENTORY];
    int inventoryCount;
    int coins;
    Shop shop;
    TrainingCamp trainingCamp;
    Pet* trainedPet;

    // Guild
    Guild playerGuild;

    // Persistent pet training stats (Dragon=0, Phoenix=1, Unicorn=2, Griffin=3)
    static const int NUM_PET_TYPES = 4;
    PetBonusStats petBonus[NUM_PET_TYPES];

    // Private constructor
    GameManager();

public:
    // Singleton access
    static GameManager* getInstance();
    void handleMenuSelection(int selection);

    // Core game functions
    void initGame();
    void runGame();

    // Screen functions
    void showPlayerNameInput();
    void showMainMenu(RenderWindow* window);
    void showShopWindow();
    void showTrainingWindow();
    void showGuildWindow();

    // Inventory management
    bool addItem(Item* item);
    bool useItem(int index, Pet* target);
    Item* getInventoryItem(int index) const;
    int getInventoryCount() const;

    // Economy
    void addCoins(int amount);
    bool spendCoins(int amount);
    int getCoins() const;

    // Training persistence
    int getPetTypeIndex(const string& name) const;
    void applyBonusStats(Pet* pet);
    void updatePetBonus(Pet* pet);

    // Save/Load (Section 7)
    void saveGame();
    bool loadGame();

    // Player info
    string getPlayerName() const;
    LeaderBoard& getLeaderBoard();

    // Utility
    void setBackground(string filename);

    // Destructor
    ~GameManager();
};

#endif 
