//Muhammad Bilal
//24I-3168
//SE-A
#include <SFML/Graphics.hpp>
#include <iostream>
#include "ArenaBattle.h"
#include "Pet.h"
#include "shot.h"

using namespace std;
using namespace sf;

    Shot::Shot(Vector2f pos, Vector2f vel, bool isFromPlayer)
        : velocity(vel), fromPlayer(isFromPlayer), active(true) {
        shape.setRadius(10.f);
        shape.setFillColor(fromPlayer ? Color::Cyan : Color::Red);
        shape.setPosition(pos);
    }

    Vector2f Shot::getPosition() const {
        return shape.getPosition();
    }

    void Shot::update() {
        if (active) shape.move(velocity);
    }

    bool Shot::checkCollision(const Sprite& target) {
        return active && shape.getGlobalBounds().intersects(target.getGlobalBounds());
    }

    void Shot::deactivate() {
        active = false;
    }

    bool Shot::isActive() const {
        return active;
    }

    bool Shot::isFromPlayer() const {
        return fromPlayer;
    }

    void Shot::draw(RenderWindow& window) const {
        if (active)
            window.draw(shape);
    }


   //Shot* ArenaBattle::shots[maxShots] = { nullptr };
    
    void ArenaBattle::showBattleText(RenderWindow& window, const string& text, float duration) {
        Text t(text, font, 24);
        t.setFillColor(Color::White);
        t.setPosition(100, 450);

        Clock clock;
        clock.restart();
        while (clock.getElapsedTime().asSeconds() < duration) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed)
                    window.close();
            }
            window.draw(t);
            window.display();
        }
    }

    int ArenaBattle::CalculateDamage(Pet* attacker, Pet* defender) {
        int baseDamage = attacker->getAttack();
        int defense = defender->getDefense();
        return max(1, baseDamage - (defense / 2));
    }

    void ArenaBattle::fireProjectile(const Vector2f& position, const Vector2f& velocity, bool fromPlayer) {
        for (int i = 0; i < maxShots; ++i) {
            if (!shots[i]) {
                shots[i] = new Shot(position, velocity, fromPlayer);
                break;
            }
        }
    }

    void ArenaBattle::draw(RenderWindow& window) {
        // Draw background
        window.draw(backgroundSprite);

        // Draw all active projectiles
        for (int i = 0; i < maxShots; ++i) {
            if (shots[i] && shots[i]->isActive()) {
                shots[i]->draw(window);
            }
        }
    }

    float ArenaBattle::distance(const Vector2f& a, const Vector2f& b) {
        float dx = a.x - b.x;
        float dy = a.y - b.y;
        return sqrt(dx * dx + dy * dy);
    }

    void ArenaBattle::updateShots(RenderWindow& window) {
        for (int i = 0; i < maxShots; ++i) {
            if (shots[i] && shots[i]->isActive()) {
                shots[i]->update();

                Vector2f pos = shots[i]->getPosition();
                if (pos.x < 0 || pos.x > window.getSize().x ||
                    pos.y < 0 || pos.y > window.getSize().y) {
                    shots[i]->deactivate();
                }
            }
        }
    }
    
    bool ArenaBattle::loadBattleAssets( string bgImage) {
        srand(std::time(0)); // Initialize random seed here (if no constructor)

        // Select a random background
        int randomIndex = rand() % NUM_BACKGROUNDS;
        string randomBg = battleBackgrounds[randomIndex];
        bgImage = randomBg;
        if (!font.loadFromFile("assets/fonts/ariblk.ttf") || !backgroundTexture.loadFromFile(bgImage)) {
            return false;
        }
        backgroundSprite.setTexture(backgroundTexture);
        backgroundSprite.setScale(
            (float)1280 / backgroundTexture.getSize().x,
            (float)720 / backgroundTexture.getSize().y
        );
        return true;
    }

    bool ArenaBattle::loadPetTexture(Texture& tex, const string& petName, const string& suffix) {
        return tex.loadFromFile("assets/images/" + petName + suffix);
    }

    void ArenaBattle::setupPetSprite(Sprite& sprite, Texture& tex, float x, float y, bool flipHorizontally) {
        sprite.setTexture(tex);
        sprite.setPosition(x, y);
        sprite.setScale(flipHorizontally ? -0.20f : 0.20f, 0.20f);
        if (flipHorizontally) {
            sprite.setOrigin((float)tex.getSize().x, 0.f);
        }
    }

    void ArenaBattle::handleProjectileFiring(const Sprite& shooter, bool facingLeft, bool isPlayerProjectile) {
        static bool keyPressed = false;
        static Clock fireClock;

        if (Keyboard::isKeyPressed(Keyboard::Space) && !keyPressed) {
            Vector2f velocity = facingLeft ? Vector2f(-8.f, 0.f) : Vector2f(8.f, 0.f);
            Vector2f startPos = shooter.getPosition();
            startPos.x += facingLeft ? -10.f : shooter.getGlobalBounds().width - 10.f;
            fireProjectile(startPos, velocity, isPlayerProjectile);
            keyPressed = true;
        }
        else if (!Keyboard::isKeyPressed(Keyboard::Space)) {
            keyPressed = false;
        }
    }

    void ArenaBattle::updatePetMovement(Sprite& sprite, float speed, bool& facingLeft, RenderWindow& window) {
        Vector2f movement(0.f, 0.f);
        bool wasFacingLeft = facingLeft;

        if (Keyboard::isKeyPressed(Keyboard::W)) movement.y -= speed;
        if (Keyboard::isKeyPressed(Keyboard::S)) movement.y += speed;
        if (Keyboard::isKeyPressed(Keyboard::A)) {
            movement.x -= speed;
            facingLeft = true;
        }
        else if (Keyboard::isKeyPressed(Keyboard::D)) {
            movement.x += speed;
            facingLeft = false;
        }

        if (facingLeft != wasFacingLeft) {
            sprite.setScale(facingLeft ? -0.20f : 0.20f, 0.20f);
            if (facingLeft) {
                sprite.setOrigin(sprite.getLocalBounds().width, 0.f);
            }
            else {
                sprite.setOrigin(0.f, 0.f);
            }
        }

        FloatRect bounds = sprite.getGlobalBounds();
        Vector2f newPos = sprite.getPosition() + movement;
        newPos.x = max(0.f, min(newPos.x, window.getSize().x - bounds.width));
        newPos.y = max(0.f, min(newPos.y, window.getSize().y - bounds.height));
        sprite.setPosition(newPos);
    }

    void ArenaBattle::updateEnemyAI(Pet* enemyPet, Sprite& enemySprite, const Sprite& playerSprite,
        RenderWindow& window, float desiredDistance) {

        Vector2f enemyPos = enemySprite.getPosition();
        Vector2f playerPos = playerSprite.getPosition();
        Vector2f toPlayer = playerPos - enemyPos;
        float distanceToPlayer = sqrt(toPlayer.x * toPlayer.x + toPlayer.y * toPlayer.y);

        bool shouldFaceRight = (playerPos.x > enemyPos.x);
        enemySprite.setScale(shouldFaceRight ? 0.2f : -0.2f, 0.2f);
        enemySprite.setOrigin(shouldFaceRight ? 0 : enemySprite.getLocalBounds().width, 0);

        if (distanceToPlayer > 0 && !enemyPet->isStunned()) {
            Vector2f direction = toPlayer / distanceToPlayer;

            if (distanceToPlayer > desiredDistance + 50) {
                enemyPos += direction * enemyPet->getSpeed();
            }
            else if (distanceToPlayer < desiredDistance - 50) {
                enemyPos -= direction * enemyPet->getSpeed() * 0.7f;
            }

            FloatRect bounds = enemySprite.getGlobalBounds();
            float minX = window.getSize().x / 2.f;
            enemyPos.x = max(minX, min(enemyPos.x, window.getSize().x - bounds.width));
            enemyPos.y = max(0.f, min(enemyPos.y, window.getSize().y - bounds.height));

            enemySprite.setPosition(enemyPos);
        }
    }

    void ArenaBattle::handleEnemyFiring(Pet* enemyPet, Sprite& enemySprite, Clock& enemyClock, const Vector2f& targetPos ,float fireRate , float projectileSpeed)
    {

        if (enemyPet->isStunned()) return;// checking stun
        if (enemyClock.getElapsedTime().asSeconds() < fireRate) return;

        bool facingRight = enemySprite.getScale().x > 0;// checkinf face right or left
        bool playerIsRight = targetPos.x > enemySprite.getPosition().x;
        if (facingRight != playerIsRight) return;

        Vector2f firePos(// fire position 
            enemySprite.getPosition().x + enemySprite.getGlobalBounds().width / 2,
            enemySprite.getPosition().y + enemySprite.getGlobalBounds().height / 2
        );

        Vector2f direction = targetPos - firePos;// direction to fire
        float length = sqrt(direction.x * direction.x + direction.y * direction.y);
        if (length <= 0) return;
        // velocity 
        Vector2f velocity = (direction / length) * projectileSpeed;
        if (facingRight) {
            velocity.x = abs(velocity.x);
        }
        else {
            velocity.x = -abs(velocity.x);
        }
        // firing function
        fireProjectile(firePos, velocity, false);
        enemyClock.restart();
    }
   


    
    // tempoty window
    void ArenaBattle::showMatchupScreen(const string teamNames[4], RenderWindow& window)
    {

        // Load background
        sf::Texture bgTexture;
        if (!bgTexture.loadFromFile("assets/images/matchup2.jpg")) {  
            cout << "Failed to load versus background" << endl;
            return;
        }
        sf::Sprite bgSprite(bgTexture);
        bgSprite.setScale(
            window.getSize().x / float(bgTexture.getSize().x),
            window.getSize().y / float(bgTexture.getSize().y)
        );

        // Load font
        sf::Font font;
        if (!font.loadFromFile("assets/fonts/ariblk.ttf")) {
            cout << "Failed to load font" << endl;
            return;
        }

        // Create versus text
        sf::Text versusText("VS", font, 120);
        versusText.setFillColor(sf::Color::White);
        versusText.setOutlineColor(sf::Color::Black);
        versusText.setOutlineThickness(3.f);
        versusText.setPosition(
            window.getSize().x / 2 - versusText.getGlobalBounds().width / 2,
            window.getSize().y / 2 - versusText.getGlobalBounds().height / 2
        );

        // Team headers
        sf::Text team1Header("TEAM 1", font, 50);
        team1Header.setFillColor(sf::Color::Blue);
        team1Header.setPosition(150, 150);

        sf::Text team2Header("TEAM 2", font, 50);
        team2Header.setFillColor(sf::Color::Red);
        team2Header.setPosition(window.getSize().x - 350.f, 150.f);

        // Team members
        sf::Text team1Member1(teamNames[0], font, 40);
        team1Member1.setFillColor(sf::Color::White);
        team1Member1.setPosition(200, 250);

        sf::Text team1Member2(teamNames[1], font, 40);
        team1Member2.setFillColor(sf::Color::White);
        team1Member2.setPosition(200, 320);

        sf::Text team2Member1(teamNames[2], font, 40);
        team2Member1.setFillColor(sf::Color::White);
        team2Member1.setPosition(window.getSize().x - 300.f, 250.f);

        sf::Text team2Member2(teamNames[3], font, 40);
        team2Member2.setFillColor(sf::Color::White);
        team2Member2.setPosition(window.getSize().x - 300.f, 320);

        // Battle start text
        sf::Text startText("BATTLE STARTING...", font, 30);
        startText.setFillColor(sf::Color::Yellow);
        startText.setPosition(
            window.getSize().x / 2 - startText.getGlobalBounds().width / 2,
            window.getSize().y - 100.f
        );

        // Animation variables
        float scale = 1.0f;
        bool growing = true;
        sf::Clock animClock;
        sf::Clock displayClock;

        while (window.isOpen() && displayClock.getElapsedTime().asSeconds() < 5.0f) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                    return;
                }
            }

            // Animate the VS text
            float dt = animClock.restart().asSeconds();
            if (growing) {
                scale += dt;
                if (scale >= 1.2f) growing = false;
            }
            else {
                scale -= dt;
                if (scale <= 1.0f) growing = true;
            }
            versusText.setScale(scale, scale);
            versusText.setPosition(
                window.getSize().x / 2 - versusText.getGlobalBounds().width / 2,
                window.getSize().y / 2 - versusText.getGlobalBounds().height / 2
            );

            // Draw everything
            window.clear();
            window.draw(bgSprite);
            window.draw(team1Header);
            window.draw(team2Header);
            window.draw(team1Member1);
            window.draw(team1Member2);
            window.draw(team2Member1);
            window.draw(team2Member2);
            window.draw(versusText);
            window.draw(startText);
            window.display();
        }
    }
    // temp match wondow
    void ArenaBattle::GetMatchScreen(const string teamNames[4], RenderWindow& window)
    {
        showMatchupScreen(teamNames, window);
    }

    void ArenaBattle::cleanupShots() {
        for (int i = 0; i < maxShots; ++i) {
            delete shots[i];
            shots[i] = nullptr;
        }
    }


   
    ArenaBattle:: ~ArenaBattle() {
        cleanupShots();
    }