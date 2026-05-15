//Muhammad Bilal
//24I-3168
//SE-A

#ifndef ArenaBattle_h
#define ArenaBattle_h

#include <SFML/Graphics.hpp>
#include "Pet.h"
#include <cstdlib>
#include <ctime>
#include "shot.h"
using namespace std;

using namespace sf;
class ArenaBattle {
protected:
    Font font;
    Texture backgroundTexture;
    Sprite backgroundSprite;
    static const int maxShots = 1000;
    Shot* shots[maxShots] = { nullptr };
    string battleBackgrounds[5] = {
        "assets/images/bg1.jpg",
        "assets/images/bg2.jpg",
        "assets/images/bg3.jpg",
        "assets/images/bg4.jpg",
       "assets/images/1v1.jpg"
    };
    const int NUM_BACKGROUNDS = 5;
   
    void showBattleText(RenderWindow& window, const string& text, float duration = 1.5f);
    // calculate damage with defend
    int CalculateDamage(Pet* attacker, Pet* defender);

    void fireProjectile(const Vector2f& position, const Vector2f& velocity, bool fromPlayer);

    void draw(RenderWindow& window);

    float distance(const Vector2f& a, const Vector2f& b);

    void updateShots(RenderWindow& window);
    bool loadBattleAssets( string bgImage);

    bool loadPetTexture(Texture& tex, const string& petName, const string& suffix);

    void setupPetSprite(Sprite& sprite, Texture& tex, float x, float y, bool flipHorizontally);

    void handleProjectileFiring(const Sprite& shooter, bool facingLeft, bool isPlayerProjectile);

    void updatePetMovement(Sprite& sprite, float speed, bool& facingLeft, RenderWindow& window);

    void updateEnemyAI(Pet* enemyPet, Sprite& enemySprite, const Sprite& playerSprite,RenderWindow& window, float desiredDistance = 300.0f);

    void handleEnemyFiring(Pet* enemyPet, Sprite& enemySprite, Clock& enemyClock, const Vector2f& targetPos, float fireRate = 1.0f, float projectileSpeed = 8.0f);

    void showMatchupScreen(const string teamNames[4], RenderWindow& window);
    

    void cleanupShots();
public:
    void GetMatchScreen(const string teamNames[4], sf::RenderWindow& window);
    virtual void start(RenderWindow& window)=0;
    virtual ~ArenaBattle();
};
#endif
