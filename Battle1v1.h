//Muhammad Bilal
//24I-3168
//SE-A

#include "Pet.h"
#include "ArenaBattle.h"
#include "HealthBar.h"
#include "GameManager.h"
#include "SFML/Graphics.hpp"

using namespace sf;

class Battle1v1 : public ArenaBattle {
private:
    Pet* playerPet;
    Pet* enemyPet;
    Sprite playerSprite;
    Sprite enemySprite;
    Texture playerTex;
    Texture enemyTex;
    bool facingLeft;

public:
    Battle1v1(Pet* player, Pet* enemy) : playerPet(player), enemyPet(enemy), facingLeft(false) {}
    // start render video load files anf textures and images
    void start(RenderWindow& window)  override {
        if (!loadBattleAssets("1v1.jpg") ||
            !loadPetTexture(playerTex, playerPet->getName(), "-right.png") ||
            !loadPetTexture(enemyTex, enemyPet->getName(), "-right.png")) {
            cout << "Failed to load assets\n";
            return;
        }

        setupPetSprite(playerSprite, playerTex, 100, 300, false);// palyerb sprite setup
        setupPetSprite(enemySprite, enemyTex, 600, 300, true);// enemy sprite setup

        Clock enemyFireClock;
        bool battleOver = false;
        // main battle lop
        while (window.isOpen() && !battleOver) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed) {
                    window.close();
                    return;
                }
            }
            // abilities cool downa and  check stunn conditoin
            playerPet->updateCoolDown();
            enemyPet->updateCoolDown();
            enemyPet->updateStun();
            playerPet->updateStun();
            // if stun and stop movemnt an dfiring
            if (!(playerPet->isStunned())) {
                updatePetMovement(playerSprite, playerPet->getSpeed(), facingLeft, window);
                handleProjectileFiring(playerSprite, facingLeft, true);
            }
            // keyboard input
            static bool SpecialAbilityPress1 = false;
            if (Keyboard::isKeyPressed(Keyboard::U)) {
                if (!SpecialAbilityPress1) {
                    if (playerPet->getName() == "Phoenix")
                    {
                        playerPet->useAbility1(nullptr);

                    }
                    else
                    playerPet->useAbility1(enemyPet);
                    SpecialAbilityPress1 = true;
                }
            }
            else {
                SpecialAbilityPress1 = false;
            }
            // special abilities
            static bool SpecialAbilityPress2 = false;
            if (Keyboard::isKeyPressed(Keyboard::I)) {
                if (!SpecialAbilityPress2) {
                    playerPet->useAbility2(enemyPet);
                    SpecialAbilityPress2 = true;
                }
            }
            else {
                SpecialAbilityPress2 = false;
            }

            // Item usage: keys 1-5 use inventory items on player pet
            GameManager* gm = GameManager::getInstance();
            static bool itemKeyPressed[5] = { false };
            Keyboard::Key itemKeys[5] = { Keyboard::Num1, Keyboard::Num2, Keyboard::Num3, Keyboard::Num4, Keyboard::Num5 };
            for (int k = 0; k < 5; k++) {
                if (Keyboard::isKeyPressed(itemKeys[k])) {
                    if (!itemKeyPressed[k] && k < gm->getInventoryCount()) {
                        gm->useItem(k, playerPet);
                        itemKeyPressed[k] = true;
                    }
                } else {
                    itemKeyPressed[k] = false;
                }
            }
            // enemy 
            Vector2f targetPos = playerSprite.getPosition();
            updateEnemyAI(enemyPet, enemySprite, playerSprite, window);// update
            handleEnemyFiring(enemyPet, enemySprite, enemyFireClock, targetPos);// firing 
            enemySpecialAbility(enemyPet, playerPet);// use of secial ability
            updateShots(window);

            checkCollisions();

            if (enemyPet->getHp() <= 0) {
                battleOver = true;
            }
            else if (playerPet->getHp() <= 0) {
                if (playerPet->getName() == "Phoenix" && static_cast<Phoenix*>(playerPet)->canRebirth()) {
                    static_cast<Phoenix*>(playerPet)->rebirth();
                }
                else {
                    battleOver = true;
                }
            }

            drawBattle(window);
        }

        showResult(window, enemyPet->getHp() <= 0);
    }

private:
    void enemySpecialAbility(Pet* enemy, Pet* player) {
        static bool abilityuse1 = false;
        static bool abilityuse2 = false;
        if (enemy->getHp() <= (enemy->getMaxHp() / 2) && !abilityuse1) {
            if (enemy->getName() == "Phoenix")
            {
                enemy->useAbility1(nullptr);
            }
            else
            enemy->useAbility1(player);
            abilityuse1 = true;
        }
        else if (enemy->getHp() <= (enemy->getMaxHp() / 4) && !abilityuse2) {
            enemy->useAbility2(player);
            abilityuse2 = true;
        }
    }
    // checking collioin and which player or enemy have to got damage
    void checkCollisions() {
        for (int i = 0; i < maxShots; i++) {
            if (shots[i] && shots[i]->isActive()) {
                if (shots[i]->isFromPlayer()) {
                    if (shots[i]->checkCollision(enemySprite)) {
                        Unicorn* unicorn = dynamic_cast<Unicorn*>(enemyPet);
                        if (unicorn && unicorn->isShieldActive()) {
                            cout << enemyPet->getName() << "'s magic shield absorbs the attack!\n";
                        }
                        else {
                            int damage = CalculateDamage(playerPet, enemyPet);
                            enemyPet->takeDamage(damage);
                        }
                        shots[i]->deactivate();
                    }
                }
                else {
                    if (shots[i]->checkCollision(playerSprite)) {
                        Unicorn* unicorn = dynamic_cast<Unicorn*>(playerPet);
                        if (unicorn && unicorn->isShieldActive()) {
                            cout << playerPet->getName() << "'s magic shield absorbs the attack!\n";
                        }
                        else {
                            int damage = CalculateDamage(enemyPet, playerPet);
                            playerPet->takeDamage(damage);
                        }
                        shots[i]->deactivate();
                    }
                }
            }
        }
    }

    void drawBattle(RenderWindow& window) {
        window.clear();
        window.draw(backgroundSprite);
        window.draw(playerSprite);
        window.draw(enemySprite);

        Font animatedfonts;
        if (animatedfonts.loadFromFile("assets/fonts/ariblk.ttf")) {
            float p_hp = static_cast<float>(playerPet->getHp());
            float e_hp = static_cast<float>(enemyPet->getHp());
            drawHealthBar(window, Vector2f(50, 30), playerPet->getHp(), playerPet->getMaxHp(),
                p_hp, Color::Green, animatedfonts);
            drawHealthBar(window, Vector2f(window.getSize().x - 300.0f, 30.0f),
                enemyPet->getHp(), enemyPet->getMaxHp(),
                e_hp, Color::Red, animatedfonts);
        }

        for (int i = 0; i < maxShots; i++) {
            if (shots[i] && shots[i]->isActive()) {
                shots[i]->draw(window);
            }
        }

        // Draw item HUD at bottom
        GameManager* gm = GameManager::getInstance();
        int invCount = gm->getInventoryCount();
        if (invCount > 0) {
            RectangleShape itemBar(Vector2f(invCount * 160.f + 20.f, 35.f));
            itemBar.setPosition(10, window.getSize().y - 45.f);
            itemBar.setFillColor(Color(0, 0, 0, 150));
            window.draw(itemBar);
            for (int i = 0; i < invCount && i < 5; i++) {
                Item* it = gm->getInventoryItem(i);
                if (it) {
                    string lbl = "[" + to_string(i + 1) + "] " + it->getName();
                    Text itemTxt(lbl, font, 14);
                    itemTxt.setFillColor(Color(255, 215, 0));
                    itemTxt.setPosition(20.f + i * 160.f, window.getSize().y - 40.f);
                    window.draw(itemTxt);
                }
            }
        }

        window.display();
    }

    void showResult(RenderWindow& window, bool playerWon) {
        Text result(playerWon ? "You Win!" : "You Lose!", font, 45);
        result.setFillColor(Color::Yellow);
        result.setPosition(
            window.getSize().x / 2 - result.getLocalBounds().width / 2,
            window.getSize().y / 2 - result.getLocalBounds().height / 2
        );

        Clock resultClock;
        // Award coins for winning and record on leaderboard
        GameManager* gm = GameManager::getInstance();
        gm->getLeaderBoard().addResult(gm->getPlayerName(), playerWon);
        if (playerWon) {
            gm->addCoins(50);
            gm->saveGame();
            result.setString("You Win! (+50 coins, Auto-saved)");
            result.setPosition(
                window.getSize().x / 2 - result.getLocalBounds().width / 2,
                window.getSize().y / 2 - result.getLocalBounds().height / 2
            );
        } else {
            gm->saveGame();
        }
        while (resultClock.getElapsedTime().asSeconds() < 3.0f && window.isOpen()) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed) {
                    window.close();
                }
            }

            window.clear();
            window.draw(backgroundSprite);
            window.draw(result);
            window.display();
        }
    }
};

