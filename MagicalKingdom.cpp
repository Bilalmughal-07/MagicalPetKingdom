//Muhammad Bilal
//24I-3168
//SE-A

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include "Pet.h"
#include "Dragon.h"
#include "Unicorn.h"
#include "Phoenix.h"
#include "Griffin.h"
#include "shot.h"
//#include "Menu.h"
#include <cmath>
//#include "Battle.h"
#include "HealthBar.h"
#include "GameManager.h"
#include "Shop.h"
#include "Item.h"
#include "sstream"

using namespace std;
using namespace sf;

void showSelectModeWindow();
//void showMatchupScreen(const string teamNames[4], RenderWindow& window);


Pet::Pet(const std::string& name, int hp, int attack, float speed, int defense)
    : name(name),
    hp(hp),
    maxHp(hp),
    attack(attack),
    speed(speed),
    defense(defense),
    level(1),
    exp(0),
    stunned(false)
{
   
}
    

// Getters implementation of Pet
string Pet::getName() const { return name; }
int Pet::getHp() const { return hp; }
int Pet::getMaxHp()const { return maxHp; }
int Pet::getAttack() const { return attack; }
float Pet::getSpeed() const { return speed; }
int Pet::getDefense() const { return defense; }
int Pet::getLevel() const { return level; }
int Pet::getExp() const { return exp; }
Vector2f Pet:: getPosition()const  { return position;}

// Setters implementation
void Pet::setName(const string& name) { this->name = name; }
void Pet::setHp(int hp) { this->hp = (hp > maxHp) ? maxHp : hp; }
void Pet::setMaxHp(int MaxHp) { this->maxHp = MaxHp; }
void Pet::setAttack(int attack) { this->attack = attack; }
void Pet::setSpeed(float speed) { this->speed = speed; }
void Pet::setDefense(int defense) { this->defense = defense; }
void Pet::setPosition(const Vector2f& pos) {
    position = pos;
}

void Pet:: stun(float duration) {
    stunned = true;
    stunDuration = duration;
    stunClock.restart();
}

void Pet::updateStun() {
    if (stunned && stunClock.getElapsedTime().asSeconds() >= stunDuration)
        stunned = false;
}

void Pet::takeDamage(int damage) {
    hp -= damage;
    if (hp < 0) hp = 0;
}

void Pet::heal(int amount) {
    hp += amount;
    if (hp > maxHp) hp = maxHp;
}

void Pet::gainExp(int amount) {
    exp += amount;
    if (exp >= 100) {
        levelUp();
        exp -= 100;
    }
}

void Pet::levelUp() {
    level++;
    maxHp += 10;
    hp = maxHp;
    attack += 5;
    defense += 3;
    speed += 2;
}

bool Pet::isStunned() const {
    return stunned;
}

void Pet::setStunned(bool status) {
    stunned = status;
}
void Pet::displayStats() const {
    cout << "Name: " << name << endl;
    cout << "Level: " << level << " | Exp: " << exp << "/100" << endl;
    cout << "HP: " << hp << "/" << maxHp << endl;
    cout << "Attack: " << attack << endl;
    cout << "Defense: " << defense << endl;
    cout << "Speed: " << speed << endl;
}

void Pet :: setLevel(int l)
{
    level = l;
}
void Pet:: setExp(int e)
{
    exp = e;
}

string Pet:: serialize() const {
    std::ostringstream oss;
    oss << name << "|"
        << hp << "|"
        << maxHp << "|"
        << attack << "|"
        << speed << "|"
        << defense << "|"
        << level << "|"
        << exp << "|"
        << position.x << "|"
        << position.y;
    return oss.str();
}

//Pet* Pet::deserialize(const std::string& data) {
//    std::istringstream iss(data);
//    std::string tokens[10]; // Fixed-size array instead of vector
//    size_t tokenCount = 0;
//
//    // Parse tokens
//    while (tokenCount < 10 && std::getline(iss, tokens[tokenCount], '|')) {
//        tokenCount++;
//    }
//
//    if (tokenCount != 10) return nullptr; // Error check
//
//    // Create Pet using protected setters
//    Pet* pet = new Pet(
//        tokens[0],                   // name
//        std::stoi(tokens[1]),       // hp
//        std::stoi(tokens[3]),       // attack
//        std::stof(tokens[4]),       // speed
//        std::stoi(tokens[5])        // defense
//    );
//
//    // Set remaining properties
//    pet->setMaxHp(std::stoi(tokens[2]));
//    pet->setLevel(std::stoi(tokens[6]));
//    pet->setExp(std::stoi(tokens[7]));
//    pet->setPosition(sf::Vector2f(
//        std::stof(tokens[8]),       // position.x
//        std::stof(tokens[9])        // position.y
//    ));
//
//    return pet;
//}


Dragon::Dragon(const  string& name)
    : Pet(name, 200, 20, 1.0f, 15), fireballAvailable(true), roarAvailable(true) {
    
}

void Dragon::useAbility1(Pet* target) {
    if (fireballAvailable) {
        fireballAttack(target);
    } else {
        cout << "Fireball is on cooldown!\n";
    }
}

void Dragon::fireballAttack(Pet* target) {
    if (!target) return;

    int damage =(this->getAttack() * 2);
    cout << name << " hurls a blazing Fireball at " << target->getName()
         << ", dealing " << damage << " damage!" << endl;

    target->takeDamage(damage);

    fireballAvailable = false;
    CooldownClock.restart();
}
void Dragon::useAbility2(Pet* enemy1, Pet* enemy2) {
    if (!roarAvailable) {
        cout << "Dragon's Roar is on cooldown!"<<endl;
        return;
    }

    dragonsRoar(enemy1, enemy2);
}

void Dragon::dragonsRoar(Pet* target1, Pet* target2) {
    if (target1) {
        target1->stun(5.0f);
        cout << name << " lets out a deafening ROAR! " << target1->getName() << " is stunned!" << endl;
    }

    if (target2) { 
        target2->stun(5.0f);
        cout << name << " lets out a deafening ROAR! " << target2->getName() << " is stunned!" << endl;
    }

    roarAvailable = false;
    roarCooldownClock.restart();
}



void Dragon::updateCoolDown() {
    // Fireball cooldown
    if (!fireballAvailable && CooldownClock.getElapsedTime().asSeconds() >= CooldownTime) {
        fireballAvailable = true;
        cout << name << "'s Fireball is ready to use again!\n";
    }

    // Dragon's Roar cooldown
    if (!roarAvailable && roarCooldownClock.getElapsedTime().asSeconds() >= RoarCooldownTime) {
        roarAvailable = true;
        cout << name << "'s Dragon's Roar is ready to use again!\n";
    }
}

void Dragon::resetCooldowns() {
    fireballAvailable = true;
    roarAvailable = true;
    cout << name << "'s abilities have been refreshed!" << endl;
}

void Dragon::displayStats() const {
    Pet::displayStats();
    cout << "Special Abilities:" << endl;
    cout << "- Fireball Attack: " << (fireballAvailable ? "Ready" : "On Cooldown") << endl;
    cout << "- Dragon's Roar: " << (roarAvailable ? "Ready" : "On Cooldown") << endl;
}

Phoenix::Phoenix(const string& name)
    : Pet(name, 150, 15, 0.6f, 25),  // High defense, medium health, slow speed
    flameHealingAvailable(true),
    rebirthAvailable(true),
    hasRebirthed(false) {
}

void Phoenix::useAbility1(Pet* target) {
    // Check if it's 1v1 (no ally)
    if (target == nullptr || target == this) {
        // 1v1 scenario - automatic healing over time
        if (flameHealingAvailable) {
            cout << name << " activates Flame Healing on itself!" << endl;
            startHealingOverTime(this, 10, 5); // 10 HP per second for 5 seconds
            flameHealingAvailable = false;
            CooldownClock.restart();
        }
    }
    else {
        // 2v2 scenario - show selection window
        showHealingSelectionWindow(target);
    }
}
void Phoenix::startHealingOverTime(Pet* target, int hpPerSecond, int durationSeconds) {
    if (!flameHealingAvailable) {
        cout << "Flame Healing is on cooldown!" << endl;
        return;
    }

    if (target == nullptr) {
        target = this;
    }

    // Store healing information
    activeHealingTarget = target;
    healingHpPerSecond = hpPerSecond;
    healingDuration = durationSeconds;
    healingTimer.restart();
    lastHealTime = 0;
    totalHealingTime = 0;
    isHealingActive = true;

    cout << name << " begins healing " << target->getName() << " over time!" << endl;
}
void Phoenix::showHealingSelectionWindow(Pet* target) {
    // Create a temporary window for ability choice
    RenderWindow abilityWindow(VideoMode(400, 200), "Phoenix Ability: Heal");

    // Load font for the buttons
    Font font;
    if (!font.loadFromFile("assets/fonts/ariblk.ttf")) {
        cout << "Failed to load font.\n";
        return;
    }

    Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("assets/images/healing.jpeg")) {
        cout << "Failed to load background image.\n";
        return;
    }

    Sprite backgroundSprite(backgroundTexture);
    backgroundSprite.setScale(
        float(abilityWindow.getSize().x) / backgroundTexture.getSize().x,
        float(abilityWindow.getSize().y) / backgroundTexture.getSize().y
    );

    RectangleShape healSelfButton(Vector2f(150, 50));
    healSelfButton.setFillColor(Color::Green);
    Text healSelfText("Yourself", font, 20);
    healSelfText.setFillColor(Color::White);

    RectangleShape healAllyButton(Vector2f(150, 50));
    healAllyButton.setFillColor(Color::Blue);
    Text healAllyText("Ally", font, 20);
    healAllyText.setFillColor(Color::White);

    // variables to track which option is selected
    int selectedOption = 1;  // 1 for "Heal Yourself", 2 for "Heal Ally"

    // center the buttons vertically
    float buttonSpacing = 10.f;
    float totalButtonHeight = healSelfButton.getSize().y + healAllyButton.getSize().y + buttonSpacing;
    float startY = (abilityWindow.getSize().y - totalButtonHeight) / 2;

    healSelfButton.setPosition(125, startY);
    healSelfText.setPosition(145, startY + 10);

    healAllyButton.setPosition(125, startY + healSelfButton.getSize().y + buttonSpacing);
    healAllyText.setPosition(175, startY + healSelfButton.getSize().y + buttonSpacing + 10);

    // Main loop to handle keyboard input
    while (abilityWindow.isOpen()) {
        Event event;
        while (abilityWindow.pollEvent(event)) {
            if (event.type == Event::Closed) {
                abilityWindow.close();
            }

            // Handle key events
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::W) {
                    selectedOption = 1;  // Select "Heal Yourself"
                }
                else if (event.key.code == Keyboard::S) {
                    selectedOption = 2;  // Select "Heal Ally"
                }
                else if (event.key.code == Keyboard::Enter) {
                    // Confirm the selection
                    if (selectedOption == 1) {
                        startHealingOverTime(this, 10, 5);  // Heal the Phoenix over time
                    }
                    else if (selectedOption == 2) {
                        startHealingOverTime(target, 10, 5);  // Heal the ally over time
                    }
                    abilityWindow.close();  // Close the ability window after action
                    flameHealingAvailable = false;
                    CooldownClock.restart();
                }
            }
        }

        // Draw everything in the temporary window
        abilityWindow.clear(Color::Black);
        abilityWindow.draw(backgroundSprite);
        abilityWindow.draw(healSelfButton);
        abilityWindow.draw(healSelfText);
        abilityWindow.draw(healAllyButton);
        abilityWindow.draw(healAllyText);

        // Highlight the selected option
        if (selectedOption == 1) {
            healSelfButton.setOutlineThickness(5);
            healSelfButton.setOutlineColor(Color::Yellow);
            healAllyButton.setOutlineThickness(0);
        }
        else if (selectedOption == 2) {
            healAllyButton.setOutlineThickness(5);
            healAllyButton.setOutlineColor(Color::Yellow);
            healSelfButton.setOutlineThickness(0);
        }

        abilityWindow.display();
    }
}



void Phoenix::updateHealingOverTime() {
    if (!isHealingActive) return;

    float elapsed = healingTimer.getElapsedTime().asSeconds();
    totalHealingTime = elapsed;

    // Check if we should heal this frame
    if (static_cast<int>(elapsed) > lastHealTime) {
        activeHealingTarget->heal(healingHpPerSecond);
        lastHealTime = static_cast<int>(elapsed);

        // Visual feedback
        cout << activeHealingTarget->getName() << " healed for " << healingHpPerSecond
            << " HP (" << lastHealTime << "/" << healingDuration << " seconds)" << endl;
    }

    // Check if healing duration is complete
    if (totalHealingTime >= healingDuration) {
        isHealingActive = false;
        cout << name << "'s healing over time effect has ended." << endl;
    }
}

void Phoenix::updateCoolDown() {
    if (!flameHealingAvailable && CooldownClock.getElapsedTime().asSeconds() >= CooldownTime) {
        flameHealingAvailable = true;
        cout << name << "'s Flame Healing is ready!\n";
    }

    // Update healing over time effect if active
    if (isHealingActive) {
        updateHealingOverTime();
    }
}

void Phoenix::resetCooldowns() {
    flameHealingAvailable = true;
    cout << name << "'s abilities have been refreshed!" << endl;
}

void Phoenix::useAbility2(Pet* target,Pet* target1 )
{
    rebirth();
}

bool Phoenix::rebirth() {
    if (hasRebirthed) {
        cout << "Rebirth can only be used once per battle!" << endl;
        return false;
    }

    if (hp > 0) {
        cout << name << " doesn't need to rebirth yet! (Current HP: " << hp << ")" << endl;
        return false;
    }

    cout << name << " bursts into flames and is reborn from the ashes!" << endl;
    hp = maxHp / 2;
    hasRebirthed = true;
    rebirthAvailable = false;
    return true;
}

void Phoenix::displayStats() const {
    Pet::displayStats();
    cout << "Special Abilities:" << endl;
    cout << "- Flame Healing: " << (flameHealingAvailable ? "Ready" : "On Cooldown") << endl;

    if (!hasRebirthed) {
        cout << "- Rebirth: " << (rebirthAvailable ? "Available" : "Used") << endl;
    }
    else {
        cout << "- Rebirth: Already used" << endl;
    }
}

bool Phoenix::canUseFlameHealing() const {
    return flameHealingAvailable;
}

bool Phoenix::canRebirth() const {

    return rebirthAvailable && !hasRebirthed;

}


void Phoenix::takeDamage(int damage) {
    Pet::takeDamage(damage);
    if (hp == 0 && !hasRebirthed) {
        rebirth();
    }
}

bool Phoenix::hasUsedRebirth() const {
    return hasRebirthed;
}

Griffin::Griffin(const std::string& name)
    : Pet(name, 130, 18, 1.3f, 18),  // Balanced stats
    clawStrikeAvailable(true),
    diveBombCooldownTime(10.0f),
    diveBombAvailable(true) {
}

void Griffin::useAbility1(Pet* target) {

    if (clawStrikeAvailable) {
        clawStrike(target);
    }
   
    else {
        cout << "Ability not available or invalid choice!" << endl;
    }
}
void Griffin::useAbility2(Pet* enemy1,Pet* enemy2) {
    if (!diveBombAvailable) {
        cout << name << "'s Dive Bomb is not ready!" << endl;
        return;
    }
  
    diveBomb(enemy1, enemy2);
}


void Griffin::clawStrike(Pet* target) {
    if (!target) return;
    int damage = getAttack() + 30;  // Strong melee hit
    cout << name << " slashes " << target->getName()
        << " with Claw Strike for " << damage << " damage" << endl;
    target->takeDamage(damage);
    clawStrikeAvailable = false;
    CooldownClock.restart();
        
}
void Griffin::updateCoolDown() {
    if (!clawStrikeAvailable && CooldownClock.getElapsedTime().asSeconds() >= CooldownTime) {
        cout << name << "'s Claw Strike is ready" << endl;
        clawStrikeAvailable = true;
    }

    if (!diveBombAvailable && diveBombCooldownClock.getElapsedTime().asSeconds() >= diveBombCooldownTime) {
        cout << name << "'s Dive Bomb is ready" << endl;
        diveBombAvailable = true;
    }
}

void Griffin::resetCooldowns() {
    clawStrikeAvailable = true;
    diveBombAvailable = true;
    cout << name << "'s abilities have been refreshed!" << endl;
}

void Griffin::diveBomb(Pet* enemy1, Pet* enemy2) {
    cout << name << " swoops down from the sky in a powerful DIVE BOMB!" << endl;

    // Apply effect to the first enemy
    if (enemy1) {
        int damage = getAttack() + 20;
        cout << name << " hits " << enemy1->getName()
            << " for " << damage << " damage and stuns them!" << endl;

        enemy1->takeDamage(damage);
        enemy1->stun(5.0f);  // 5 second stun
    }

    // Apply effect to the second enemy
    if (enemy2) {
        int damage = getAttack() + 20;
        cout << name << " hits " << enemy2->getName()
            << " for " << damage << " damage and stuns them!" << endl;

        enemy2->takeDamage(damage);
        enemy2->stun(5.0f);  //  5 second stun
    }

    diveBombAvailable = false;
    diveBombCooldownClock.restart();
}


void Griffin::displayStats() const {
    Pet::displayStats();
    cout << "Special Abilities:" << endl;
    cout << "- Claw Strike: " << (clawStrikeAvailable ? "Ready" : "On Cooldown") << endl;
    cout << "- Dive Bomb: " << (diveBombAvailable ? "Ready" : "On Cooldown") << endl;
}

bool Griffin::canUseClawStrike() const {
    return clawStrikeAvailable;
}

bool Griffin::canUseDiveBomb() const {
    return diveBombAvailable;
}

void Griffin::resetAbilities() {
    clawStrikeAvailable = true;
    diveBombAvailable = true;
}


Unicorn::Unicorn(const std::string& name)
    : Pet(name, 100, 15, 1.5, 12),  // High speed, low health
    speedBurstAvailable(true),
    speedBoostActive(false),
    magicShieldAvailable(true),
    shieldActive(false),
    shieldDuration(5.0) {
}
void Unicorn::useAbility1(Pet* target) {
    if (speedBurstAvailable) {
        activateSpeedBurst();
    }
    else {
        cout << name << "'s Speed Burst is not ready!" << endl;
    }
}

void Unicorn::useAbility2(Pet* target,Pet* target1) {
    if (magicShieldAvailable && !shieldActive) {
        activateMagicShield();
    }
    else if (shieldActive) {
        cout << name << "'s shield is already active!" << endl;
    }
    else {
        cout << name << "'s Magic Shield is not ready!" << endl;
    }
}

void Unicorn::activateSpeedBurst() {
    cout << name << " glows with magical energy and gains incredible speed!" << endl;
    speed += 1.5f;
    speedBurstAvailable = false;
    speedBoostActive = true;
    speedBoostclock.restart();
    CooldownClock.restart();
}

void Unicorn::activateMagicShield() {
    cout << name << " creates a shimmering protective shield!" << endl;
    shieldActive = true;
    magicShieldAvailable = false;
    shieldTimer.restart();
    shieldCooldownClock.restart();
}

void Unicorn::displayStats() const {
    Pet::displayStats();
    cout << "Special Abilities:" << endl;
    cout << "- Speed Burst: " << ( speedBurstAvailable? "Ready" : "On Cooldown") << endl;
    cout << "- Magical Shield: " << ( magicShieldAvailable? "Ready" : "On Cooldown") << endl;
}
void Unicorn::deactivateShield() {
    if (shieldActive) {
        cout << name << "'s magic shield fades away." << endl;
        shieldActive = false;
    }
}

void Unicorn::updateCoolDown() {
    // Speed Burst cooldown reset
    if (speedBoostActive && speedBoostclock.getElapsedTime().asSeconds() >= speedBoostduration) {
        speed -= 1.5f;
        speedBoostActive = false;
        cout << "Speed reduces to normal" << endl;
    }

    if (!speedBurstAvailable && CooldownClock.getElapsedTime().asSeconds() >= CooldownTime) {
        speedBurstAvailable = true;
        cout << name << "'s Speed Burst is ready!" << endl;
    }

    // Magic Shield duration check
    if (shieldActive && shieldTimer.getElapsedTime().asSeconds() >= shieldDuration) {
        deactivateShield();
    }

    // Magic Shield cooldown reset
    if (!magicShieldAvailable && shieldCooldownClock.getElapsedTime().asSeconds() >= shieldCooldownTime) {
        magicShieldAvailable = true;
        cout << name << "'s Magic Shield is ready!" << endl;
    }
}

void Unicorn::takeDamage(int damage) {
    if (shieldActive) {
        damage = 0;
        cout << "The magic shield absorbs  damage!" << endl;
    }
    Pet::takeDamage(damage);
}

bool Unicorn::isShieldActive() const {
    return shieldActive;
}

void Unicorn::resetCooldowns() {
    speedBurstAvailable = true;
    magicShieldAvailable = true;
    cout << name << "'s abilities have been refreshed!" << endl;
}









int main() {
    
        try {
             //GameManager object
            GameManager* game = GameManager::getInstance();
            
            // Run the game - this starts the main game loop
            game->runGame();

            // Clean up
            delete game;
        }
        catch (const exception& e) {
           cerr << "Fatal error: " << e.what() <<endl;
            return 1;
        }

    
  
}