//Muhammad Bilal
//24I-3168
//SE-A


#ifndef PET_H
#define PET_H

#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
using namespace std;
using namespace sf;

class Pet{
    
protected:
    string name;
    int hp;
    int maxHp;
    int attack;
    float speed;
    int defense;
    int level;
    int exp;
    Vector2f position;
    Clock CooldownClock;  // Tracks cooldown time
    float CooldownTime = 10.0f;
    bool stunned = false;
    Clock stunClock;
    float stunDuration = 0.0f;

public:
    Pet(const string& name, int hp, int attack, float speed, int defense);
    virtual ~Pet() {}

    // Getters
    string getName() const;
    int getHp() const;
     int getMaxHp()const ;
    int getAttack() const;
    float getSpeed() const;
    int getDefense() const;
    int getLevel() const;
    int getExp() const;
    bool isStunned() const;
    virtual Vector2f getPosition() const;
    
    

    // Setters
    void setName(const std::string& name);
    void setHp(int hp);
    void setMaxHp(int maxHp);
    void setAttack(int attack);
    void setSpeed(float speed);
    void setDefense(int defense);
    void setLevel(int level);
    void setExp(int e);
    void setStunned(bool status);
    virtual void setPosition(const Vector2f& pos);

    // pure Virtual Function 
    virtual void useAbility1(Pet* target) = 0;
    virtual void useAbility2(Pet* target,Pet* target2= nullptr ) = 0;
    virtual void updateCoolDown() {};
    virtual void resetCooldowns() {};  // Reset all ability cooldowns (used by Mana Potion)
    virtual void updateStun();
    void stun(float duration);
  
    virtual void takeDamage(int damage);
    void heal(int amount);
   
    void levelUp();
    void gainExp(int amount);

    // Display
    virtual void displayStats() const;

    string serialize() const;
    static Pet* deserialize(const string& data);
};

#endif