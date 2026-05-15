//Muhammad Bilal
//24I-3168
//SE-A
class Battle2v2 : public ArenaBattle {
private:
    Pet* playerTeam[2];
    Pet* enemyTeam[2];
    Sprite playerSprites[2];
    Sprite enemySprites[2];
    Texture playerTextures[2];
    Texture enemyTextures[2];
    bool facingLeft[2];
    bool playerAlive[2];
    bool enemyAlive[2];

public:
    Battle2v2(Pet* player1, Pet* player2, Pet* enemy1, Pet* enemy2) {
        playerTeam[0] = player1;
        playerTeam[1] = player2;
        enemyTeam[0] = enemy1;
        enemyTeam[1] = enemy2;
        facingLeft[0] = facingLeft[1] = false;
        playerAlive[0] = playerAlive[1] = true;
        enemyAlive[0] = enemyAlive[1] = true;
    }

    void start(RenderWindow& window) override
    {// setup image
        if (!loadBattleAssets("1v1.jpg") ||
            !loadPetTexture(playerTextures[0], playerTeam[0]->getName(), "-right.png") ||
            !loadPetTexture(playerTextures[1], playerTeam[1]->getName(), "-right.png") ||
            !loadPetTexture(enemyTextures[0], enemyTeam[0]->getName(), "-right.png") ||
            !loadPetTexture(enemyTextures[1], enemyTeam[1]->getName(), "-right.png")) {
            cout << "Failed to load assets\n";
            return;
        }
        // setup sprite
        setupPetSprite(playerSprites[0], playerTextures[0], 100, 200, false);
        setupPetSprite(playerSprites[1], playerTextures[1], 100, 400, false);
        setupPetSprite(enemySprites[0], enemyTextures[0], 600, 200, true);
        setupPetSprite(enemySprites[1], enemyTextures[1], 600, 400, true);

        Clock enemyFireClocks[2];
        static Clock c;
        bool battleOver = false;
        // main battle loop
        while (window.isOpen() && !battleOver) {
            Event e;
            while (window.pollEvent(e)) {
                if (e.type == Event::Closed) {
                    window.close();
                }
            }
            //upadte conditoin
            updatePlayerTeam(window, c);
            updateEnemyTeam(window, enemyFireClocks);
            updateShots(window);

            checkCollisions();
            updateAliveStatus();

            bool playerTeamDefeated = !playerAlive[0] && !playerAlive[1];
            bool enemyTeamDefeated = !enemyAlive[0] && !enemyAlive[1];
            // wining conditoin
            if (playerTeamDefeated || enemyTeamDefeated) {
                battleOver = true;
                showTeamResult(window, enemyTeamDefeated);
            }

            drawBattle(window);
        }
    }

private:
    void updatePlayerTeam(RenderWindow& window, Clock& c) {
        // Player 1 controls (WASD + Space)
        if (playerAlive[0]) {
            updatePetMovement(playerSprites[0], playerTeam[0]->getSpeed(), facingLeft[0], window);
            handleProjectileFiring(playerSprites[0], facingLeft[0], true);
        }

        // Player 2 controls (Arrow keys + Numpad0)
        if (playerAlive[1]) {
            updatePlayer2Movement(window);
            handlePlayer2Firing(c);
        }

        handlePlayerAbilities();

        for (int i = 0; i < 2; i++) {
            playerTeam[i]->updateCoolDown();
        }
    }

    void updatePlayer2Movement(RenderWindow& window) {
        Vector2f movement(0.f, 0.f);
        float speed = playerTeam[1]->getSpeed();
        // keyboard input and movement
        if (Keyboard::isKeyPressed(Keyboard::Up)) movement.y -= speed;
        if (Keyboard::isKeyPressed(Keyboard::Down)) movement.y += speed;
        if (Keyboard::isKeyPressed(Keyboard::Left)) {
            movement.x -= speed;
            facingLeft[1] = true;
        }
        else if (Keyboard::isKeyPressed(Keyboard::Right)) {
            movement.x += speed;
            facingLeft[1] = false;
        }

        if (facingLeft[1]) {
            playerSprites[1].setScale(-0.20f, 0.20f);
            playerSprites[1].setOrigin(playerSprites[1].getLocalBounds().width, 0.f);
        }
        else {
            playerSprites[1].setScale(0.20f, 0.20f);
            playerSprites[1].setOrigin(0.f, 0.f);
        }

        FloatRect bounds = playerSprites[1].getGlobalBounds();
        Vector2f newPos = playerSprites[1].getPosition() + movement;
        newPos.x = max(0.f, min(newPos.x, window.getSize().x - bounds.width));
        newPos.y = max(0.f, min(newPos.y, window.getSize().y - bounds.height));
        playerSprites[1].setPosition(newPos);
    }
    // firing for palyer 2 same s player 1
    void handlePlayer2Firing(Clock& c) {
        static bool keyPressed2 = false;
        
        if (Keyboard::isKeyPressed(Keyboard::L)) {
            if (!keyPressed2 && c.getElapsedTime().asSeconds() > 0.5f) {
                Vector2f velocity = facingLeft[1] ? Vector2f(-8.f, 0.f) : Vector2f(8.f, 0.f);
                Vector2f startPos = playerSprites[1].getPosition();
                startPos.x += facingLeft[1] ? -10.f : playerSprites[1].getGlobalBounds().width - 10.f;
                fireProjectile(startPos, velocity, true);
                c.restart();
                keyPressed2 = true;
            }
        }
        else {
            keyPressed2 = false;
        }
    }
    // handling special abilities
    void handlePlayerAbilities() {
        static bool ability1Pressed[2] = { false, false };
        static bool ability2Pressed[2] = { false, false };

        // Player 1 abilities
        if (Keyboard::isKeyPressed(Keyboard::U) && !ability1Pressed[0]) {
            
            if (playerTeam[0]->getName() == "Phoenix")
            {
                playerTeam[0]->useAbility1(playerTeam[1]);
            }
            else {
                Pet* target = getRandomAliveEnemy();
                if (target) playerTeam[0]->useAbility1(target);
            }
            ability1Pressed[0] = true;
        }
        else if (!Keyboard::isKeyPressed(Keyboard::U)) {
            ability1Pressed[0] = false;
        }
        // checking player and call abilities
        if (Keyboard::isKeyPressed(Keyboard::I) && !ability2Pressed[0]) {
            if (playerTeam[0]->getName() == "Phoenix") {
                playerTeam[0]->useAbility2(playerTeam[0]);
            }
            else if (playerTeam[0]->getName() == "Griffin" || playerTeam[0]->getName() == "Dragon") {
                playerTeam[0]->useAbility2(enemyTeam[0], enemyTeam[1]);
            }
            else {
                Pet* target = getRandomAliveEnemy();
                if (target) playerTeam[0]->useAbility2(target);
            }
            ability2Pressed[0] = true;
        }
        else if (!Keyboard::isKeyPressed(Keyboard::I)) {
            ability2Pressed[0] = false;
        }

        // Player 2 abilities
        if (Keyboard::isKeyPressed(Keyboard::N) && !ability1Pressed[1]) {
            if (playerTeam[1]->getName() == "Phoenix")
            {
                playerTeam[1]->useAbility1(playerTeam[0]);
            }
            else {
                Pet* target = getRandomAliveEnemy();
                if (target) playerTeam[0]->useAbility1(target);
            }
            ability1Pressed[1] = true;
        }
        else if (!Keyboard::isKeyPressed(Keyboard::N)) {
            ability1Pressed[1] = false;
        }

        if (Keyboard::isKeyPressed(Keyboard::M) && !ability2Pressed[1]) {
            if (playerTeam[1]->getName() == "Phoenix") {
                playerTeam[1]->useAbility2(playerTeam[1]);
            }
            else if (playerTeam[1]->getName() == "Griffin" || playerTeam[0]->getName() == "Dragon") {
                playerTeam[1]->useAbility2(enemyTeam[0], enemyTeam[1]);
            }
            else {
                Pet* target = getRandomAliveEnemy();
                if (target) playerTeam[1]->useAbility2(target);
            }
            ability2Pressed[1] = true;
        }
        else if (!Keyboard::isKeyPressed(Keyboard::M)) {
            ability2Pressed[1] = false;
        }
    }

   // random enemy
    Pet* getRandomAliveEnemy() {
        bool alive0 = enemyAlive[0];
        bool alive1 = enemyAlive[1];

        if (!alive0 && !alive1) return nullptr;
        if (alive0 && !alive1) return enemyTeam[0];
        if (!alive0 && alive1) return enemyTeam[1];

        // both alive
        return enemyTeam[rand() % 2];
    }

    void updateEnemyTeam(RenderWindow& window, Clock enemyFireClocks[]) {
        for (int i = 0; i < 2; i++) {
            if (enemyAlive[i]) {
                enemyTeam[i]->updateCoolDown();
                enemyTeam[i]->updateStun();

                Sprite* targetSprite = getClosestPlayerSprite(enemySprites[i]);
                if (targetSprite) {
                    updateEnemyAI(enemyTeam[i], enemySprites[i], *targetSprite, window);
                    handleEnemyFiring(enemyTeam[i], enemySprites[i], enemyFireClocks[i],
                        targetSprite->getPosition());
                }
            }
        }
    }

    Sprite* getClosestPlayerSprite(const Sprite& enemySprite) {
        Sprite* closest = nullptr;
        float minDistance = numeric_limits<float>::max();

        for (int i = 0; i < 2; i++) {
            if (playerAlive[i]) {
                float dist = distance(enemySprite.getPosition(), playerSprites[i].getPosition());
                if (dist < minDistance) {
                    minDistance = dist;
                    closest = &playerSprites[i];
                }
            }
        }
        return closest;
    }

    void checkCollisions() {
        for (int i = 0; i < maxShots; i++) {
            if (shots[i] && shots[i]->isActive()) {
                if (shots[i]->isFromPlayer()) {
                    for (int j = 0; j < 2; j++) {
                        if (enemyAlive[j] && shots[i]->checkCollision(enemySprites[j])) {
                            handleEnemyHit(j, shots[i]);
                        }
                    }
                }
                else {
                    for (int j = 0; j < 2; j++) {
                        if (playerAlive[j] && shots[i]->checkCollision(playerSprites[j])) {
                            handlePlayerHit(j, shots[i]);
                        }
                    }
                }
            }
        }
    }
    // enemy hittig fire
    void handleEnemyHit(int enemyIndex, Shot* shot) {
        Unicorn* unicorn = dynamic_cast<Unicorn*>(enemyTeam[enemyIndex]);
        if (unicorn && unicorn->isShieldActive()) {
            cout << enemyTeam[enemyIndex]->getName() << "'s magic shield absorbs the attack!\n";
        }
        else {
            Pet* attacker = playerTeam[rand() % 2];
            int damage = CalculateDamage(attacker, enemyTeam[enemyIndex]);
            enemyTeam[enemyIndex]->takeDamage(damage);
        }
        shot->deactivate();
    }

    void handlePlayerHit(int playerIndex, Shot* shot) {
        Unicorn* unicorn = dynamic_cast<Unicorn*>(playerTeam[playerIndex]);
        if (unicorn && unicorn->isShieldActive()) {
            cout << playerTeam[playerIndex]->getName() << "'s magic shield absorbs the attack!\n";
        }
        else {
            Pet* attacker = enemyTeam[rand() % 2];
            int damage = CalculateDamage(attacker, playerTeam[playerIndex]);
            playerTeam[playerIndex]->takeDamage(damage);
        }
        shot->deactivate();
    }
    //checking and update alive status
    void updateAliveStatus() {
        for (int i = 0; i < 2; i++) {
            if (playerTeam[i]->getHp() <= 0 && playerAlive[i]) {
                playerAlive[i] = false;
            }
            if (enemyTeam[i]->getHp() <= 0 && enemyAlive[i]) {
                enemyAlive[i] = false;
            }
        }
    }
    // draw and render 
    void drawBattle(RenderWindow& window) {
        window.clear();
        window.draw(backgroundSprite);

        for (int i = 0; i < 2; i++) {
            if (playerAlive[i]) window.draw(playerSprites[i]);
            if (enemyAlive[i]) window.draw(enemySprites[i]);
        }

        Font animatedfonts;
        if (animatedfonts.loadFromFile("assets/fonts/ariblk.ttf")) {
            // Player team health bars
            for (int i = 0; i < 2; i++) {
                float hp = static_cast<float>(playerTeam[i]->getHp());
                drawHealthBar(window, Vector2f(50, 30 + i * 40), playerTeam[i]->getHp(),
                    playerTeam[i]->getMaxHp(), hp, Color::Green, animatedfonts);
            }

            // Enemy team health bars
            for (int i = 0; i < 2; i++) {
                float hp = static_cast<float>(enemyTeam[i]->getHp());
                drawHealthBar(window, Vector2f(static_cast<float> (window.getSize().x) - 300.f, 30.f + i * 40),
                    enemyTeam[i]->getHp(), enemyTeam[i]->getMaxHp(),
                    hp, Color::Red, animatedfonts);
            }
        }

        for (int i = 0; i < maxShots; ++i) {
            if (shots[i] && shots[i]->isActive()) {
                shots[i]->draw(window);
            }
        }

        window.display();
    }
    // shoeing results
    void showTeamResult(RenderWindow& window, bool playerWon) {
        window.clear();
        window.draw(backgroundSprite);

        // Record result on leaderboard and guild
        GameManager* gm = GameManager::getInstance();
        gm->getLeaderBoard().addResult(gm->getPlayerName(), playerWon);
        if (playerWon) {
            gm->addCoins(75); // More coins for 2v2 wins
        }
        gm->saveGame();

        Text result(playerWon ? "Your Team Wins! (+75 coins)" : "Enemy Team Wins!", font, 45);
        result.setFillColor(Color::Yellow);
        result.setPosition(
            window.getSize().x / 2 - result.getLocalBounds().width / 2,
            window.getSize().y / 2 - result.getLocalBounds().height / 2
        );
        window.draw(result);

        window.display();

        Clock clock;
        while (clock.getElapsedTime().asSeconds() < 3.0f) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed)
                    window.close();
            }
        }
    }
};
