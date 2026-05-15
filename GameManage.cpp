//Muhammad Bilal
//24I-3168
//SE-A

#include "GameManager.h"
#include <iostream>
#include "Pet.h"
#include "Dragon.h"
#include "Unicorn.h"
#include "Phoenix.h"
#include "Menu.h"
#include <cmath>
#include "ArenaBattle.h"
#include "Battle1v1.h"
#include "Battle2v2.h"
#include "Griffin.h"
#include "LeaderBoard.h"
#include "Item.h"
#include "Shop.h"
#include "Training.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <fstream>
#include <sstream>



using namespace std;
using namespace sf;



Pet* SelectPet(const string name)
{
    Pet* pet = nullptr;
    if (name == "Unicorn")
        pet = new Unicorn(name);
    else if (name == "Dragon")
        pet = new Dragon(name);
    else if (name == "Phoenix")
        pet = new Phoenix(name);
    else if (name == "Griffin")
        pet = new Griffin(name);
    // Apply training bonus stats
    if (pet) {
        GameManager::getInstance()->applyBonusStats(pet);
    }
    return pet;
}


void selectPetWithImages(sf::RenderWindow& window, bool singlemode) {
    const int totalPets = 4;
    const string petNames[totalPets] = { "Dragon", "Phoenix", "Unicorn", "Griffin" };
    const string petImageFiles[totalPets] = { "assets/images/Dragon-right.png", "assets/images/Phoenix-right.png", "assets/images/Unicorn-right.png", "assets/images/Griffin-right.png" };

    // Load background
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("assets/images/PetSelection.jpeg")) {
        cout << "Failed to load background image." << endl;
        return;
    }
    sf::Sprite backgroundSprite(backgroundTexture);
    backgroundSprite.setScale(
        static_cast<float>(window.getSize().x) / backgroundTexture.getSize().x,
        static_cast<float>(window.getSize().y) / backgroundTexture.getSize().y
    );

    // Load font
    Font font;
    if (!font.loadFromFile("assets/fonts/ariblk.ttf")) {
        cout << "Failed to load font." << endl;
        return;
    }

    sf::Text title("Select Your Magical Pet", font, 40);
    title.setFillColor(sf::Color::Yellow);
    title.setPosition(window.getSize().x / 2.f - title.getLocalBounds().width / 2, 30);

    // Load pet textures and sprites
    sf::Texture petTextures[totalPets];
    sf::Sprite petSprites[totalPets];
    const float scaleFactor = 0.5f;
    const float spacing = 80.0f;

    for (int i = 0; i < totalPets; i++) {
        if (!petTextures[i].loadFromFile(petImageFiles[i])) {
            cout << "Failed to load image: " << petImageFiles[i] << endl;
            return;
        }
        petSprites[i].setTexture(petTextures[i]);
        petSprites[i].setScale(scaleFactor, scaleFactor);
    }

    // Position sprites in 2x2 grid
    sf::Vector2f spriteSize(
        petSprites[0].getGlobalBounds().width,
        petSprites[0].getGlobalBounds().height
    );
    float totalWidth = 2 * spriteSize.x + spacing;
    float totalHeight = 2 * spriteSize.y + spacing;
    float startX = (window.getSize().x - totalWidth) / 2.0f;
    float startY = (window.getSize().y - totalHeight) / 2.0f + 50;

    for (int i = 0; i < totalPets; i++) {
        int row = i / 2;
        int col = i % 2;
        petSprites[i].setPosition(
            startX + col * (spriteSize.x + spacing),
            startY + row * (spriteSize.y + spacing)
        );
    }

    // Selection variables
    int selectedIndex = 0;
    int selections = 0;
    string selectedPetNames[4];
    bool petChosen = false;
    sf::Clock animClock;

    while (window.isOpen() && !petChosen) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }
            // keyboard input
            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                case sf::Keyboard::Up:
                    if (selectedIndex >= 2) selectedIndex -= 2;
                    break;
                case sf::Keyboard::Down:
                    if (selectedIndex <= 1) selectedIndex += 2;
                    break;
                case sf::Keyboard::Left:
                    if (selectedIndex % 2 != 0) selectedIndex -= 1;
                    break;
                case sf::Keyboard::Right:
                    if (selectedIndex % 2 == 0) selectedIndex += 1;
                    break;
                case sf::Keyboard::Enter:
                    selectedPetNames[selections] = petNames[selectedIndex];
                    selections++;
                    cout << "Selected Pet: " << petNames[selectedIndex] << endl;
                    // 1v1 check
                    if (!singlemode) {
                        if (selections == 2) {
                            Pet* playerPet = SelectPet(selectedPetNames[0]);
                            Pet* enemyPet = SelectPet(selectedPetNames[1]);

                            if (playerPet && enemyPet) {
                                RenderWindow battleWindow(sf::VideoMode(1280, 720), "Battle", sf::Style::Close);
                                ArenaBattle* battle = new Battle1v1(playerPet, enemyPet);
                                battle->start(battleWindow);
                                delete battle;

                                delete playerPet;
                                delete enemyPet;
                                petChosen = true;
                            }
                        }
                    }
                    else {// 2v2 check
                        if (selections == 4) {
                            Pet* team1[2] = {
                                SelectPet(selectedPetNames[0]),
                                SelectPet(selectedPetNames[1])
                            };
                            Pet* team2[2] = {
                                SelectPet(selectedPetNames[2]),
                                SelectPet(selectedPetNames[3])
                            };

                            if (team1[0] && team1[1] && team2[0] && team2[1]) {

                                // make window and call
                                ArenaBattle* battle = new Battle2v2(team1[0], team1[1], team2[0], team2[1]);
                                RenderWindow matchupWindow(sf::VideoMode(1280, 720), "Matchup", sf::Style::Close);
                                battle->GetMatchScreen(selectedPetNames, matchupWindow);
                                RenderWindow battleWindow(sf::VideoMode(1280, 720), "Team Battle", sf::Style::Close);

                                battle->start(battleWindow);
                                delete battle;
                                /*startTeamBattle(team1, team2, battleWindow);*/

                                for (int i = 0; i < 2; i++) {
                                    delete team1[i];
                                    team1[i] = nullptr;
                                    delete team2[i];
                                    team2[i] = nullptr;
                                }
                                petChosen = true;
                            }
                        }
                    }
                    break;
                case sf::Keyboard::Escape:
                    window.close();
                    return;
                }
            }
        }

        // Animation
        float time = animClock.getElapsedTime().asSeconds();
        float scaleAnim = 1.0f + 0.05f * std::sin(3.0f * time);

        // Draw
        window.clear();
        window.draw(backgroundSprite);
        window.draw(title);

        for (int i = 0; i < totalPets; ++i) {
            // Highlight selected pet
            if (i == selectedIndex) {
                petSprites[i].setScale(scaleFactor * scaleAnim, scaleFactor * scaleAnim);

                sf::FloatRect bounds = petSprites[i].getGlobalBounds();
                sf::RectangleShape highlight;
                highlight.setSize({ bounds.width + 20, bounds.height + 20 });
                highlight.setPosition(petSprites[i].getPosition().x - 10, petSprites[i].getPosition().y - 10);
                highlight.setFillColor(sf::Color(255, 255, 0, 100));
                window.draw(highlight);
            }
            else {
                petSprites[i].setScale(scaleFactor, scaleFactor);
            }

            window.draw(petSprites[i]);
        }

        window.display();
    }
}
// made selection
void showSelectModeWindow() {
    RenderWindow modeWindow(VideoMode(1280, 720), "Magical Pets Kingdom - Select Mode", Style::Default);


    // font load
    Font font;
    if (!font.loadFromFile("assets/fonts/ariblk.ttf")) {
        cout << "Failed to load font.\n";
        return;
    }
    /* SetBackground("selectmode.jpeg", modeWindow);*/
    // textures
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("assets/images/selectmode.jpeg")) {
        cout << "Failed to load background image." << endl;
    }
    sf::Sprite backgroundSprite(backgroundTexture);
    sf::Vector2u windowSize = modeWindow.getSize();
    sf::Vector2u bgSize = backgroundTexture.getSize();
    backgroundSprite.setScale(
        static_cast<float>(windowSize.x) / bgSize.x,
        static_cast<float>(windowSize.y) / bgSize.y
    );

    string options[2] = { "Single Player", "Two Player" };
    Text buttons[2];



    for (int i = 0; i < 2; i++) {
        buttons[i].setFont(font);
        buttons[i].setString(options[i]);
        buttons[i].setCharacterSize(45);
        buttons[i].setPosition(500.0, 300 + i * 100.0);
        buttons[i].setFillColor(Color::White);
    }

    int selected = 0;
    Clock clock;
    // main loop
    while (modeWindow.isOpen()) {
        Event m;
        while (modeWindow.pollEvent(m)) {
            if (m.type == Event::Closed)
                modeWindow.close();

            if (m.type == Event::KeyPressed) {
                if (m.key.code == Keyboard::Up || m.key.code == Keyboard::W)
                    selected = (selected - 1 + 2) % 2;
                else if (m.key.code == Keyboard::Down || m.key.code == Keyboard::S)
                    selected = (selected + 1) % 2;
                else if (m.key.code == Keyboard::Enter) {
                    if (selected == 0) {
                        RenderWindow window(VideoMode(1280, 720), "Magical Pets Kingdom - Select Character", Style::Default);
                        selectPetWithImages(window, 0);

                    }
                    else {
                        RenderWindow window(VideoMode(1280, 720), "Magical Pets Kingdom - Select Character", Style::Default);
                        selectPetWithImages(window, 1);
                    }

                }
                else if (m.key.code == Keyboard::Escape)
                {
                    cout << "Returning to Main Menu." << endl;
                    modeWindow.close();
                }
            }
        }

        float t = clock.getElapsedTime().asSeconds();
        float scalePulse = 1.0f + 0.1f * sin(3 * t);

        modeWindow.clear(Color(30, 30, 60));
        modeWindow.draw(backgroundSprite);
        for (int i = 0; i < 2; ++i) {
            if (i == selected) {
                buttons[i].setFillColor(Color::Yellow);
                buttons[i].setScale(scalePulse, scalePulse);
            }
            else {
                buttons[i].setFillColor(Color::White);
                buttons[i].setScale(1.f, 1.f);
            }
            modeWindow.draw(buttons[i]);
        }

        modeWindow.display();

    }
}


GameManager* GameManager::instance = nullptr;

GameManager::GameManager() {
    window = nullptr;
    gameRunning = false;
    nameEntered = false;
    playerName = "";
    coins = 100;
    inventoryCount = 0;
    trainedPet = nullptr;
    for (int i = 0; i < MAX_INVENTORY; i++) inventory[i] = nullptr;
}

GameManager* GameManager::getInstance() {
    if (instance == nullptr) {
        instance = new GameManager();
    }
    return instance;
}
void GameManager::setBackground(string filename) {// background
    static sf::Texture bgTexture;
    static sf::Sprite bgSprite;

    if (!bgTexture.loadFromFile(filename)) {
        std::cerr << "Failed to load background: " << filename << std::endl;
        return;
    }

    bgSprite.setTexture(bgTexture);

    // Scale the background to fit the window
    sf::Vector2u windowSize = window->getSize();
    sf::Vector2u textureSize = bgTexture.getSize();

    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;

    bgSprite.setScale(scaleX, scaleY);

    window->draw(bgSprite);
}
void GameManager::initGame() {
    // Create window
    window = new RenderWindow(VideoMode(800, 600), "Magical Pets Kingdom");
    window->setFramerateLimit(60);

    // Load font
    if (!font.loadFromFile("assets/fonts/ariblk.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
    }

    gameRunning = true;
}

void GameManager::runGame() {
    initGame();

    while (gameRunning && window->isOpen()) {
        if (!nameEntered) {
            showPlayerNameInput();
        }
        else {
          
            try {
                Menu menu(*window);  // Use existing window

                int result = menu.run();
                    cout << result;
                    window = nullptr;
                     window = new RenderWindow(VideoMode(1280, 720), "Magical Pets Kingdom");
                    switch (result) {
                    case 0: // New Game
                        showMainMenu(window);
                        break;
                    case 1: // Load Game
                        if (loadGame()) {
                            cout << "Game loaded successfully!" << endl;
                        } else {
                            cout << "No save file found. Starting fresh." << endl;
                        }
                        showMainMenu(window);
                        break;
                    case 2: // Game Modes
                        showMainMenu(window);
                        break;
                    case 3: // Leaderboard
                        lb.createWindow();
                        lb.run();
                        break;
                    case 4: // Exit
                        window->close();
                        gameRunning = false;
                        break;
                    case -1: // Window closed
                        window->close();
                        gameRunning = false;
                        break;
                    }

                  //  menu.reset(); // Prepare menu for next use
               // }
            }
            catch (const exception& e) {
                cerr << "Error: " << e.what() << endl;
                cerr << "An error occurred. Please try again." << endl;
                
            }
        }
    }
}

void GameManager::showPlayerNameInput() {// Player name to enter in game
    window->clear(sf::Color(30, 30, 60));
    setBackground("assets/images/background.png");

    // Title
    sf::Text title("Enter Your Name:", font, 40);
    title.setPosition(250, 200);
    title.setFillColor(sf::Color::White);

    // Player name text
    sf::Text nameText(playerName + "_", font, 36);
    nameText.setPosition(300, 250);
    nameText.setFillColor(sf::Color::Yellow);

    // Instruction
    sf::Text instruction("Press Enter to continue", font, 24);
    instruction.setPosition(270, 320);
    instruction.setFillColor(sf::Color::White);

    // Handle events
    sf::Event event;
    while (window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window->close();
            gameRunning = false;
        }

        if (event.type == sf::Event::TextEntered) {
            if (event.text.unicode == '\b' && !playerName.empty()) {
                playerName.pop_back(); // Backspace
            }
            else if (event.text.unicode < 128 && event.text.unicode != '\r') {
                playerName += static_cast<char>(event.text.unicode);
            }
            nameText.setString(playerName + "_");
        }

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Enter && !playerName.empty()) {
                nameEntered = true;
            }
        }
    }

    // Draw everything
    window->draw(title);
    window->draw(nameText);
    window->draw(instruction);
    window->display();
}

void GameManager::exitGame(sf::RenderWindow* window) {
    window->close(); // Simply call close() on the window
}


void GameManager::showMainMenu(RenderWindow* window) {
    if (window) {
        window->close();
        delete window;
    }

    // Create new window
    window = new RenderWindow(VideoMode(1280, 720), "Main Menu");

    // Load background
    Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("assets/images/background.png")) {
        cerr << "Failed to load background image!\n";
        window->close();
        return;
    }

    Sprite background(backgroundTexture);

    // Scale background to fit window
    Vector2u textureSize = backgroundTexture.getSize();
    Vector2u windowSize = window->getSize();
    float scaleX = (float)windowSize.x / textureSize.x;
    float scaleY = (float)windowSize.y / textureSize.y;
    background.setScale(scaleX, scaleY);

    // Load font
    if (!font.loadFromFile("assets/fonts/ariblk.ttf")) {
        cerr << "Failed to load font!\n";
        window->close();
        return;
    }

    // Load sound
    SoundBuffer buffer;
    if (!buffer.loadFromFile("assets/audio/hover.wav")) {
        cerr << "Failed to load click sound!\n";
    }
    Sound clickSound;
    clickSound.setBuffer(buffer);

    // Optional: Background music
    Music bgMusic;
    if (bgMusic.openFromFile("assets/audio/bs_music.wav")) {
        bgMusic.setLoop(true);
        bgMusic.play();
    }

    // Setup UI elements
    Text title("Main Menu", font, 50);
    title.setFillColor(Color::White);

    Text welcome("Welcome, " + playerName + "!", font, 30);
    welcome.setFillColor(Color::Yellow);

    string options[5] = { "Arena", "Training", "Guild", "Shop", "Exit" };
    Text menuItems[5];
    FloatRect menuItemBounds[5];

    // Temporary for calculating size
    for (int i = 0; i < 5; i++) {
        menuItems[i].setFont(font);
        menuItems[i].setString(options[i]);
        menuItems[i].setCharacterSize(36);
    }

    // Centering positions
    FloatRect titleBounds = title.getLocalBounds();
    title.setOrigin(titleBounds.width / 2, titleBounds.height / 2);
    title.setPosition(windowSize.x / 2, 80);

    FloatRect welcomeBounds = welcome.getLocalBounds();
    welcome.setOrigin(welcomeBounds.width / 2, welcomeBounds.height / 2);
    welcome.setPosition(windowSize.x / 2, 150);

    for (int i = 0; i < 5; i++) {
        FloatRect bounds = menuItems[i].getLocalBounds();
        menuItems[i].setOrigin(bounds.width / 2, bounds.height / 2);
        menuItems[i].setPosition(windowSize.x / 2, 250 + i * 60);
        menuItemBounds[i] = menuItems[i].getGlobalBounds();
    }

    int selectedItem = 0;
    Clock clock;

    // Main menu loop
    while (window->isOpen() && gameRunning) {
        Event event;
        while (window->pollEvent(event)) {
            if (event.type == Event::Closed) {
                window->close();
                gameRunning = false;
            }

            // Handle input
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Up) {
                    selectedItem = (selectedItem - 1 + 5) % 5;
                    clickSound.play();
                }
                else if (event.key.code == Keyboard::Down) {
                    selectedItem = (selectedItem + 1) % 5;
                    clickSound.play();
                }
                else if (event.key.code == Keyboard::Enter) {
                    clickSound.play();
                    handleMenuSelection(selectedItem);
                }
                else if (event.key.code == Keyboard::Escape) {
                    window->close();
                    gameRunning = false;
                }
            }

            // Mouse handling
            if (event.type == Event::MouseMoved) {
                Vector2f mousePos = window->mapPixelToCoords(Vector2i(event.mouseMove.x, event.mouseMove.y));
                for (int i = 0; i < 5; i++) {
                    if (menuItems[i].getGlobalBounds().contains(mousePos)) {
                        selectedItem = i;
                        break;
                    }
                }
            }

            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                Vector2f mousePos = window->mapPixelToCoords(Vector2i(event.mouseButton.x, event.mouseButton.y));
                for (int i = 0; i < 5; i++) {
                    if (menuItems[i].getGlobalBounds().contains(mousePos)) {
                        clickSound.play();
                        handleMenuSelection(i);
                        break;
                    }
                }
            }
        }

        // Animation pulse
        float time = clock.getElapsedTime().asSeconds();
        float pulse = sin(time * 3.0f) * 0.2f + 1.0f;

        // Drawing
        window->clear();
        window->draw(background);
        window->draw(title);
        window->draw(welcome);

        for (int i = 0; i < 5; i++) {
            if (i == selectedItem) {
                menuItems[i].setFillColor(Color::Yellow);
                menuItems[i].setScale(pulse, pulse);
            }
            else {
                menuItems[i].setFillColor(Color::White);
                menuItems[i].setScale(1.f, 1.f);
            }
            window->draw(menuItems[i]);
        }

        window->display();
    }

    if (bgMusic.getStatus() == Music::Playing) {
        bgMusic.stop();
    }
}

void GameManager::handleMenuSelection(int selection) {
    switch (selection) {
    case 0: // Arena
        showSelectModeWindow();
        break;
    case 1: // Training
        showTrainingWindow();
        break;
    case 2: // Guild
        showGuildWindow();
        break;
    case 3: // Shop
        showShopWindow();
        break;
    case 4: // Exit
        window->close();
        gameRunning = false;
        break;
    }
}

// Inventory management
bool GameManager::addItem(Item* item) {
    if (inventoryCount >= MAX_INVENTORY) return false;
    inventory[inventoryCount++] = item;
    return true;
}

bool GameManager::useItem(int index, Pet* target) {
    if (index < 0 || index >= inventoryCount || !target) return false;
    inventory[index]->use(target);
    if (inventory[index]->getQuantity() <= 0) {
        delete inventory[index];
        for (int i = index; i < inventoryCount - 1; i++) {
            inventory[i] = inventory[i + 1];
        }
        inventory[inventoryCount - 1] = nullptr;
        inventoryCount--;
    }
    return true;
}

Item* GameManager::getInventoryItem(int index) const {
    if (index < 0 || index >= inventoryCount) return nullptr;
    return inventory[index];
}

int GameManager::getInventoryCount() const { return inventoryCount; }
void GameManager::addCoins(int amount) { coins += amount; }
bool GameManager::spendCoins(int amount) {
    if (coins >= amount) { coins -= amount; return true; }
    return false;
}
int GameManager::getCoins() const { return coins; }
string GameManager::getPlayerName() const { return playerName; }
LeaderBoard& GameManager::getLeaderBoard() { return lb; }

// Training Window (SFML GUI)
void GameManager::showTrainingWindow() {
    RenderWindow trainWin(VideoMode(1280, 720), "Training Camp", Style::Close);
    Font tFont;
    if (!tFont.loadFromFile("assets/fonts/ariblk.ttf")) return;
    Texture bgTex;
    if (!bgTex.loadFromFile("assets/images/background.png")) return;
    Sprite bgSprite(bgTex);
    bgSprite.setScale(1280.f / bgTex.getSize().x, 720.f / bgTex.getSize().y);
    SoundBuffer hBuf; Sound hSound;
    if (hBuf.loadFromFile("assets/audio/hover.wav")) hSound.setBuffer(hBuf);

    const int totalPets = 4;
    string petNames[totalPets] = { "Dragon", "Phoenix", "Unicorn", "Griffin" };
    Texture petTex[totalPets]; Sprite petSpr[totalPets];
    string petImgs[totalPets] = {
        "assets/images/Dragon-right.png", "assets/images/Phoenix-right.png",
        "assets/images/Unicorn-right.png", "assets/images/Griffin-right.png"
    };
    for (int i = 0; i < totalPets; i++) {
        petTex[i].loadFromFile(petImgs[i]);
        petSpr[i].setTexture(petTex[i]);
        petSpr[i].setScale(0.35f, 0.35f);
        petSpr[i].setPosition(100.f + i * 280.f, 300.f);
    }

    int phase = 0;
    int sel = 0;
    string msg = "";
    Clock msgClock, animClock;

    while (trainWin.isOpen()) {
        Event ev;
        while (trainWin.pollEvent(ev)) {
            if (ev.type == Event::Closed) { trainWin.close(); return; }
            if (ev.type == Event::KeyPressed) {
                if (ev.key.code == Keyboard::Escape) {
                    if (phase == 1) { phase = 0; sel = 0; }
                    else { trainWin.close(); return; }
                }
                if (phase == 0) {
                    if (ev.key.code == Keyboard::Left) { sel = (sel - 1 + totalPets) % totalPets; hSound.play(); }
                    if (ev.key.code == Keyboard::Right) { sel = (sel + 1) % totalPets; hSound.play(); }
                    if (ev.key.code == Keyboard::Enter) {
                        if (trainedPet) delete trainedPet;
                        if (petNames[sel] == "Dragon") trainedPet = new Dragon(petNames[sel]);
                        else if (petNames[sel] == "Phoenix") trainedPet = new Phoenix(petNames[sel]);
                        else if (petNames[sel] == "Unicorn") trainedPet = new Unicorn(petNames[sel]);
                        else trainedPet = new Griffin(petNames[sel]);
                        phase = 1; sel = 0;
                    }
                }
                else {
                    if (ev.key.code == Keyboard::Up) { sel = (sel - 1 + TRAINING_COUNT) % TRAINING_COUNT; hSound.play(); }
                    if (ev.key.code == Keyboard::Down) { sel = (sel + 1) % TRAINING_COUNT; hSound.play(); }
                    if (ev.key.code == Keyboard::Enter) {
                        bool ok = trainingCamp.trainPet(trainedPet, (TrainingType)sel, coins);
                        if (ok) {
                            updatePetBonus(trainedPet);
                            saveGame(); // Auto-save after training
                            msg = "Training complete! +25 EXP (Auto-saved)";
                        } else {
                            msg = "Not enough coins!";
                        }
                        msgClock.restart();
                    }
                }
            }
        }

        if (msgClock.getElapsedTime().asSeconds() > 2.5f) msg = "";
        float t = animClock.getElapsedTime().asSeconds();
        float pulse = 1.0f + 0.08f * sin(3.0f * t);

        trainWin.clear();
        trainWin.draw(bgSprite);
        RectangleShape overlay(Vector2f(1280, 720));
        overlay.setFillColor(Color(0, 0, 0, 160));
        trainWin.draw(overlay);

        Text title("TRAINING CAMP", tFont, 50);
        title.setFillColor(Color(255, 200, 50));
        title.setPosition(1280.f / 2 - title.getLocalBounds().width / 2, 20);
        trainWin.draw(title);

        Text coinTxt("Coins: " + to_string(coins), tFont, 24);
        coinTxt.setFillColor(Color(255, 215, 0));
        coinTxt.setPosition(50, 90);
        trainWin.draw(coinTxt);

        if (phase == 0) {
            Text sub("Select a Pet to Train", tFont, 30);
            sub.setFillColor(Color::White);
            sub.setPosition(1280.f / 2 - sub.getLocalBounds().width / 2, 130);
            trainWin.draw(sub);
            for (int i = 0; i < totalPets; i++) {
                if (i == sel) {
                    RectangleShape hl(Vector2f(petSpr[i].getGlobalBounds().width + 20, petSpr[i].getGlobalBounds().height + 20));
                    hl.setPosition(petSpr[i].getPosition().x - 10, petSpr[i].getPosition().y - 10);
                    hl.setFillColor(Color(255, 255, 0, 60));
                    hl.setOutlineThickness(2); hl.setOutlineColor(Color::Yellow);
                    trainWin.draw(hl);
                    petSpr[i].setScale(0.35f * pulse, 0.35f * pulse);
                } else {
                    petSpr[i].setScale(0.35f, 0.35f);
                }
                trainWin.draw(petSpr[i]);
                Text nm(petNames[i], tFont, 20);
                nm.setFillColor(i == sel ? Color::Yellow : Color::White);
                nm.setPosition(petSpr[i].getPosition().x + 20, petSpr[i].getPosition().y + petSpr[i].getGlobalBounds().height + 5);
                trainWin.draw(nm);
            }
            Text ctrl("[Left/Right] Select  |  [Enter] Choose  |  [ESC] Back", tFont, 16);
            ctrl.setFillColor(Color(180, 180, 180));
            ctrl.setPosition(1280.f / 2 - ctrl.getLocalBounds().width / 2, 690);
            trainWin.draw(ctrl);
        }
        else {
            Text petTitle(trainedPet->getName() + " (Lvl " + to_string(trainedPet->getLevel()) + ")", tFont, 28);
            petTitle.setFillColor(Color::Yellow);
            petTitle.setPosition(60, 130);
            trainWin.draw(petTitle);
            string stats[] = {
                "HP: " + to_string(trainedPet->getHp()) + "/" + to_string(trainedPet->getMaxHp()),
                "Attack: " + to_string(trainedPet->getAttack()),
                "Speed: " + to_string((int)trainedPet->getSpeed()),
                "Defense: " + to_string(trainedPet->getDefense()),
                "EXP: " + to_string(trainedPet->getExp()) + "/100"
            };
            for (int i = 0; i < 5; i++) {
                Text st(stats[i], tFont, 20);
                st.setFillColor(Color(200, 230, 255));
                st.setPosition(80, 175 + i * 32);
                trainWin.draw(st);
            }
            Text sub("Choose Training (Cost: " + to_string(trainingCamp.getCost()) + " coins)", tFont, 26);
            sub.setFillColor(Color::White);
            sub.setPosition(500, 130);
            trainWin.draw(sub);
            for (int i = 0; i < TRAINING_COUNT; i++) {
                TrainingType tt = (TrainingType)i;
                RectangleShape bg(Vector2f(650, 55));
                bg.setPosition(500, 190 + i * 75);
                if (i == sel) {
                    bg.setFillColor(Color(255, 255, 0, 40));
                    bg.setOutlineThickness(2); bg.setOutlineColor(Color::Yellow);
                } else {
                    bg.setFillColor(Color(255, 255, 255, 15));
                }
                trainWin.draw(bg);
                Text nm(trainingCamp.getTrainingName(tt), tFont, 22);
                nm.setFillColor(i == sel ? Color::Yellow : Color::White);
                if (i == sel) nm.setScale(pulse, pulse);
                nm.setPosition(515, 195 + i * 75);
                trainWin.draw(nm);
                Text desc(trainingCamp.getTrainingDescription(tt), tFont, 14);
                desc.setFillColor(Color(180, 180, 180));
                desc.setPosition(515, 222 + i * 75);
                trainWin.draw(desc);
                Text gain(trainingCamp.getStatGain(tt), tFont, 18);
                gain.setFillColor(Color(100, 255, 100));
                gain.setPosition(1020, 200 + i * 75);
                trainWin.draw(gain);
            }
            Text ctrl("[Up/Down] Select  |  [Enter] Train  |  [ESC] Back", tFont, 16);
            ctrl.setFillColor(Color(180, 180, 180));
            ctrl.setPosition(1280.f / 2 - ctrl.getLocalBounds().width / 2, 690);
            trainWin.draw(ctrl);
        }

        if (!msg.empty()) {
            Text mt(msg, tFont, 24);
            mt.setFillColor(Color::Green);
            mt.setPosition(1280.f / 2 - mt.getLocalBounds().width / 2, 650);
            trainWin.draw(mt);
        }
        trainWin.display();
    }
}

// Guild Window (SFML GUI)
void GameManager::showGuildWindow() {
    RenderWindow gWin(VideoMode(1280, 720), "Guild Management", Style::Close);
    Font gFont;
    if (!gFont.loadFromFile("assets/fonts/ariblk.ttf")) return;
    Texture bgTex;
    if (!bgTex.loadFromFile("assets/images/background.png")) return;
    Sprite bgSpr(bgTex);
    bgSpr.setScale(1280.f / bgTex.getSize().x, 720.f / bgTex.getSize().y);
    SoundBuffer hBuf; Sound hSnd;
    if (hBuf.loadFromFile("assets/audio/hover.wav")) hSnd.setBuffer(hBuf);

    // If guild has no name, set player's name
    if (playerGuild.getName().empty()) {
        playerGuild.setName(playerName + "'s Guild");
    }

    string petOptions[] = { "Dragon", "Phoenix", "Unicorn", "Griffin" };
    int sel = 0; // 0-3 = guild pets, 4-7 = add pet options
    int mode = 0; // 0=view guild, 1=add pet
    string msg = "";
    Clock msgClk, animClk;

    while (gWin.isOpen()) {
        Event ev;
        while (gWin.pollEvent(ev)) {
            if (ev.type == Event::Closed) { gWin.close(); return; }
            if (ev.type == Event::KeyPressed) {
                if (ev.key.code == Keyboard::Escape) {
                    if (mode == 1) { mode = 0; sel = 0; }
                    else { gWin.close(); return; }
                }
                if (mode == 0) {
                    int cnt = playerGuild.getPetCount();
                    if (ev.key.code == Keyboard::Up && cnt > 0) { sel = (sel - 1 + cnt) % cnt; hSnd.play(); }
                    if (ev.key.code == Keyboard::Down && cnt > 0) { sel = (sel + 1) % cnt; hSnd.play(); }
                    if (ev.key.code == Keyboard::A) { mode = 1; sel = 0; }
                    if (ev.key.code == Keyboard::R && cnt > 0) {
                        msg = "Removed " + playerGuild.getPetName(sel);
                        playerGuild.removePet(sel);
                        if (sel >= playerGuild.getPetCount() && playerGuild.getPetCount() > 0)
                            sel = playerGuild.getPetCount() - 1;
                        saveGame();
                        msgClk.restart();
                    }
                    if (ev.key.code == Keyboard::C && cnt > 0) {
                        PetRole cur = playerGuild.getPetRole(sel);
                        PetRole next = (PetRole)((cur + 1) % ROLE_COUNT);
                        playerGuild.setPetRole(sel, next);
                        msg = playerGuild.getPetName(sel) + " -> " + playerGuild.getRoleName(next);
                        saveGame();
                        msgClk.restart();
                    }
                }
                else { // Add pet mode
                    if (ev.key.code == Keyboard::Up) { sel = (sel - 1 + 4) % 4; hSnd.play(); }
                    if (ev.key.code == Keyboard::Down) { sel = (sel + 1) % 4; hSnd.play(); }
                    if (ev.key.code == Keyboard::Enter) {
                        if (playerGuild.addPet(petOptions[sel], ROLE_DAMAGE_DEALER)) {
                            msg = "Added " + petOptions[sel] + " to guild!";
                            saveGame();
                        } else {
                            msg = "Already in guild or guild full!";
                        }
                        mode = 0; sel = 0;
                        msgClk.restart();
                    }
                }
            }
        }

        if (msgClk.getElapsedTime().asSeconds() > 2.5f) msg = "";
        float pulse = 1.0f + 0.08f * sin(3.0f * animClk.getElapsedTime().asSeconds());

        gWin.clear();
        gWin.draw(bgSpr);
        RectangleShape ov(Vector2f(1280, 720));
        ov.setFillColor(Color(0, 0, 0, 160));
        gWin.draw(ov);

        Text title(playerGuild.getName(), gFont, 45);
        title.setFillColor(Color(255, 200, 50));
        title.setPosition(1280.f / 2 - title.getLocalBounds().width / 2, 20);
        gWin.draw(title);

        Text wl("Wins: " + to_string(playerGuild.getWins()) + "  Losses: " + to_string(playerGuild.getLosses()), gFont, 22);
        wl.setFillColor(Color(200, 200, 200));
        wl.setPosition(50, 85);
        gWin.draw(wl);

        if (mode == 0) {
            Text sub("Guild Members (" + to_string(playerGuild.getPetCount()) + "/4)", gFont, 28);
            sub.setFillColor(Color::White);
            sub.setPosition(80, 130);
            gWin.draw(sub);

            int cnt = playerGuild.getPetCount();
            if (cnt == 0) {
                Text empty("No pets yet! Press [A] to add a pet.", gFont, 22);
                empty.setFillColor(Color(200, 200, 200));
                empty.setPosition(350, 350);
                gWin.draw(empty);
            }
            for (int i = 0; i < cnt; i++) {
                RectangleShape bg(Vector2f(900, 60));
                bg.setPosition(190, 185 + i * 80);
                if (i == sel) {
                    bg.setFillColor(Color(255, 255, 0, 40));
                    bg.setOutlineThickness(2); bg.setOutlineColor(Color::Yellow);
                } else {
                    bg.setFillColor(Color(255, 255, 255, 15));
                }
                gWin.draw(bg);

                Text nm(playerGuild.getPetName(i), gFont, 24);
                nm.setFillColor(i == sel ? Color::Yellow : Color::White);
                if (i == sel) nm.setScale(pulse, pulse);
                nm.setPosition(210, 192 + i * 80);
                gWin.draw(nm);

                Text role("[" + playerGuild.getRoleName(playerGuild.getPetRole(i)) + "]", gFont, 18);
                role.setFillColor(Color(100, 200, 255));
                role.setPosition(450, 198 + i * 80);
                gWin.draw(role);
            }

            Text ctrl("[A] Add Pet  |  [R] Remove  |  [C] Cycle Role  |  [ESC] Back", gFont, 16);
            ctrl.setFillColor(Color(180, 180, 180));
            ctrl.setPosition(1280.f / 2 - ctrl.getLocalBounds().width / 2, 690);
            gWin.draw(ctrl);
        }
        else {
            Text sub("Select a Pet to Add", gFont, 28);
            sub.setFillColor(Color::White);
            sub.setPosition(1280.f / 2 - sub.getLocalBounds().width / 2, 130);
            gWin.draw(sub);
            for (int i = 0; i < 4; i++) {
                RectangleShape bg(Vector2f(500, 55));
                bg.setPosition(390, 200 + i * 70);
                if (i == sel) {
                    bg.setFillColor(Color(255, 255, 0, 40));
                    bg.setOutlineThickness(2); bg.setOutlineColor(Color::Yellow);
                } else {
                    bg.setFillColor(Color(255, 255, 255, 15));
                }
                gWin.draw(bg);
                Text nm(petOptions[i], gFont, 24);
                nm.setFillColor(i == sel ? Color::Yellow : Color::White);
                if (i == sel) nm.setScale(pulse, pulse);
                nm.setPosition(410, 210 + i * 70);
                gWin.draw(nm);
            }
            Text ctrl("[Enter] Add  |  [ESC] Cancel", gFont, 16);
            ctrl.setFillColor(Color(180, 180, 180));
            ctrl.setPosition(1280.f / 2 - ctrl.getLocalBounds().width / 2, 690);
            gWin.draw(ctrl);
        }

        if (!msg.empty()) {
            Text mt(msg, gFont, 24);
            mt.setFillColor(Color::Green);
            mt.setPosition(1280.f / 2 - mt.getLocalBounds().width / 2, 650);
            gWin.draw(mt);
        }
        gWin.display();
    }
}

// Shop Window (SFML GUI)
void GameManager::showShopWindow() {
    RenderWindow shopWindow(VideoMode(1280, 720), "Magical Pets Kingdom - Shop", Style::Close);

    Font shopFont;
    if (!shopFont.loadFromFile("assets/fonts/ariblk.ttf")) { cout << "Failed to load font." << endl; return; }

    Texture bgTex;
    if (!bgTex.loadFromFile("assets/images/background.png")) { cout << "Failed to load bg." << endl; return; }
    Sprite bgSprite(bgTex);
    bgSprite.setScale(1280.f / bgTex.getSize().x, 720.f / bgTex.getSize().y);

    SoundBuffer hoverBuf;
    Sound hoverSound;
    if (hoverBuf.loadFromFile("assets/audio/hover.wav")) hoverSound.setBuffer(hoverBuf);

    int selected = 0;
    string message = "";
    Clock msgClock, animClock;

    while (shopWindow.isOpen()) {
        Event ev;
        while (shopWindow.pollEvent(ev)) {
            if (ev.type == Event::Closed) { shopWindow.close(); return; }
            if (ev.type == Event::KeyPressed) {
                int itemCount = shop.getItemCount();
                if (ev.key.code == Keyboard::Up) {
                    selected = (selected - 1 + itemCount) % max(1, itemCount);
                    hoverSound.play();
                }
                else if (ev.key.code == Keyboard::Down) {
                    selected = (selected + 1) % max(1, itemCount);
                    hoverSound.play();
                }
                else if (ev.key.code == Keyboard::Enter && itemCount > 0) {
                    Item* bought = shop.purchaseItem(selected, coins);
                    if (bought) {
                        addItem(bought);
                        message = "Purchased " + bought->getName() + "!";
                        if (selected >= shop.getItemCount() && shop.getItemCount() > 0)
                            selected = shop.getItemCount() - 1;
                    } else {
                        message = "Not enough coins!";
                    }
                    msgClock.restart();
                }
                else if (ev.key.code == Keyboard::R) {
                    shop.restock();
                    selected = 0;
                    message = "Shop restocked!";
                    msgClock.restart();
                }
                else if (ev.key.code == Keyboard::Escape) {
                    shopWindow.close(); return;
                }
            }
        }

        if (msgClock.getElapsedTime().asSeconds() > 2.5f) message = "";
        float t = animClock.getElapsedTime().asSeconds();
        float pulse = 1.0f + 0.08f * sin(3.0f * t);

        shopWindow.clear();
        shopWindow.draw(bgSprite);

        // Semi-transparent overlay
        RectangleShape overlay(Vector2f(1280, 720));
        overlay.setFillColor(Color(0, 0, 0, 150));
        shopWindow.draw(overlay);

        // Title
        Text title("SHOP", shopFont, 55);
        title.setFillColor(Color::Yellow);
        title.setPosition(1280.f / 2 - title.getLocalBounds().width / 2, 30);
        shopWindow.draw(title);

        // Coins display
        Text coinText("Coins: " + to_string(coins), shopFont, 28);
        coinText.setFillColor(Color(255, 215, 0));
        coinText.setPosition(50, 100);
        shopWindow.draw(coinText);

        // Inventory count
        Text invText("Inventory: " + to_string(inventoryCount) + "/" + to_string(MAX_INVENTORY), shopFont, 22);
        invText.setFillColor(Color(200, 200, 200));
        invText.setPosition(50, 140);
        shopWindow.draw(invText);

        // Shop items
        int itemCount = shop.getItemCount();
        float startY = 200;
        for (int i = 0; i < itemCount; i++) {
            Item* it = shop.getItem(i);
            RectangleShape itemBg(Vector2f(900, 55));
            itemBg.setPosition(190, startY + i * 70);
            if (i == selected) {
                itemBg.setFillColor(Color(255, 255, 0, 40));
                itemBg.setOutlineThickness(2);
                itemBg.setOutlineColor(Color::Yellow);
            } else {
                itemBg.setFillColor(Color(255, 255, 255, 15));
            }
            shopWindow.draw(itemBg);

            string label = it->getName() + "  -  " + it->getDescription();
            Text itemText(label, shopFont, 20);
            itemText.setFillColor(i == selected ? Color::Yellow : Color::White);
            if (i == selected) itemText.setScale(pulse, pulse);
            else itemText.setScale(1.f, 1.f);
            itemText.setPosition(200, startY + i * 70 + 8);
            shopWindow.draw(itemText);

            Text priceText(to_string(it->getPrice()) + " coins  [x" + to_string(it->getQuantity()) + "]", shopFont, 18);
            priceText.setFillColor(Color(255, 215, 0));
            priceText.setPosition(950, startY + i * 70 + 12);
            shopWindow.draw(priceText);
        }

        if (itemCount == 0) {
            Text empty("Shop is empty! Press R to restock.", shopFont, 26);
            empty.setFillColor(Color(200, 200, 200));
            empty.setPosition(400, 350);
            shopWindow.draw(empty);
        }

        // Message
        if (!message.empty()) {
            Text msgText(message, shopFont, 26);
            msgText.setFillColor(Color::Green);
            msgText.setPosition(1280.f / 2 - msgText.getLocalBounds().width / 2, 650);
            shopWindow.draw(msgText);
        }

        // Controls
        Text controls("[Enter] Buy  |  [R] Restock  |  [ESC] Back", shopFont, 18);
        controls.setFillColor(Color(180, 180, 180));
        controls.setPosition(1280.f / 2 - controls.getLocalBounds().width / 2, 690);
        shopWindow.draw(controls);

        shopWindow.display();
    }
}

// --- Training Persistence ---
int GameManager::getPetTypeIndex(const string& name) const {
    if (name == "Dragon") return 0;
    if (name == "Phoenix") return 1;
    if (name == "Unicorn") return 2;
    if (name == "Griffin") return 3;
    return -1;
}

void GameManager::applyBonusStats(Pet* pet) {
    if (!pet) return;
    int idx = getPetTypeIndex(pet->getName());
    if (idx < 0) return;
    PetBonusStats& b = petBonus[idx];
    pet->setMaxHp(pet->getMaxHp() + b.bonusHp);
    pet->setHp(pet->getMaxHp());
    pet->setAttack(pet->getAttack() + b.bonusAttack);
    pet->setSpeed(pet->getSpeed() + b.bonusSpeed);
    pet->setDefense(pet->getDefense() + b.bonusDefense);
    pet->setLevel(b.level);
    pet->setExp(b.exp);

    // Apply guild role bonuses
    for (int i = 0; i < playerGuild.getPetCount(); i++) {
        if (playerGuild.getPetName(i) == pet->getName()) {
            PetRole role = playerGuild.getPetRole(i);
            switch (role) {
            case ROLE_TANK:
                pet->setMaxHp(pet->getMaxHp() + pet->getMaxHp() / 5); // +20% HP
                pet->setHp(pet->getMaxHp());
                cout << pet->getName() << " [Tank] +20% HP!" << endl;
                break;
            case ROLE_HEALER:
                pet->heal(10); // +10 HP heal
                cout << pet->getName() << " [Healer] +10 HP!" << endl;
                break;
            case ROLE_DAMAGE_DEALER:
                pet->setAttack(pet->getAttack() + pet->getAttack() * 15 / 100); // +15% ATK
                cout << pet->getName() << " [Damage Dealer] +15% ATK!" << endl;
                break;
            }
            break;
        }
    }

    cout << pet->getName() << " loaded with bonuses! (HP+" << b.bonusHp
         << " ATK+" << b.bonusAttack << " SPD+" << b.bonusSpeed
         << " DEF+" << b.bonusDefense << ")" << endl;
}

void GameManager::updatePetBonus(Pet* pet) {
    if (!pet) return;
    int idx = getPetTypeIndex(pet->getName());
    if (idx < 0) return;
    PetBonusStats& b = petBonus[idx];
    // Calculate bonus as difference from base stats
    int baseHp = 0, baseAtk = 0, baseDef = 0;
    float baseSpd = 0;
    if (pet->getName() == "Dragon")   { baseHp = 200; baseAtk = 20; baseSpd = 1.0f; baseDef = 15; }
    if (pet->getName() == "Phoenix")  { baseHp = 150; baseAtk = 15; baseSpd = 0.6f; baseDef = 25; }
    if (pet->getName() == "Unicorn")  { baseHp = 100; baseAtk = 15; baseSpd = 1.5f; baseDef = 12; }
    if (pet->getName() == "Griffin")  { baseHp = 130; baseAtk = 18; baseSpd = 1.3f; baseDef = 18; }
    b.bonusHp = pet->getMaxHp() - baseHp;
    b.bonusAttack = pet->getAttack() - baseAtk;
    b.bonusSpeed = pet->getSpeed() - baseSpd;
    b.bonusDefense = pet->getDefense() - baseDef;
    b.level = pet->getLevel();
    b.exp = pet->getExp();
}

// --- Save/Load System (Section 7) ---
void GameManager::saveGame() {
    try {
        ofstream file("savegame.txt");
        if (!file.is_open()) {
            cerr << "Failed to open save file!" << endl;
            return;
        }
        file << "PLAYER|" << playerName << endl;
        file << "COINS|" << coins << endl;
        file << "INVENTORY|" << inventoryCount << endl;
        for (int i = 0; i < inventoryCount; i++) {
            file << "ITEM|" << inventory[i]->getName() << "|" << inventory[i]->getQuantity() << endl;
        }
        string petTypeNames[4] = { "Dragon", "Phoenix", "Unicorn", "Griffin" };
        for (int i = 0; i < NUM_PET_TYPES; i++) {
            file << "PETBONUS|" << petTypeNames[i] << "|"
                 << petBonus[i].bonusHp << "|"
                 << petBonus[i].bonusAttack << "|"
                 << petBonus[i].bonusSpeed << "|"
                 << petBonus[i].bonusDefense << "|"
                 << petBonus[i].level << "|"
                 << petBonus[i].exp << endl;
        }
        // Save guild data
        file << "GUILD|" << playerGuild.getName() << "|" << playerGuild.getWins() << "|" << playerGuild.getLosses() << endl;
        for (int i = 0; i < playerGuild.getPetCount(); i++) {
            file << "GUILDPET|" << playerGuild.getPetName(i) << "|" << (int)playerGuild.getPetRole(i) << endl;
        }
        file.close();
        cout << "Game saved successfully!" << endl;
    }
    catch (const exception& e) {
        cerr << "Save error: " << e.what() << endl;
    }
}

bool GameManager::loadGame() {
    try {
        ifstream file("savegame.txt");
        if (!file.is_open()) return false;

        string line;
        // Clear current inventory
        for (int i = 0; i < inventoryCount; i++) {
            delete inventory[i];
            inventory[i] = nullptr;
        }
        inventoryCount = 0;

        while (getline(file, line)) {
            istringstream iss(line);
            string tag;
            getline(iss, tag, '|');

            if (tag == "PLAYER") {
                getline(iss, playerName);
                nameEntered = true;
            }
            else if (tag == "COINS") {
                string val; getline(iss, val);
                coins = stoi(val);
            }
            else if (tag == "ITEM") {
                string name, qty;
                getline(iss, name, '|');
                getline(iss, qty);
                Item* item = nullptr;
                if (name == "Healing Potion") item = new HealingPotion();
                else if (name == "Mana Potion") item = new ManaPotion();
                else if (name == "Speed Boost") item = new SpeedBoost();
                else if (name == "Attack Boost") item = new AttackBoost();
                else if (name == "Defense Shield") item = new DefenseShield();
                if (item) {
                    item->setQuantity(stoi(qty));
                    addItem(item);
                }
            }
            else if (tag == "PETBONUS") {
                string name, hp, atk, spd, def, lvl, xp;
                getline(iss, name, '|');
                getline(iss, hp, '|');
                getline(iss, atk, '|');
                getline(iss, spd, '|');
                getline(iss, def, '|');
                getline(iss, lvl, '|');
                getline(iss, xp);
                int idx = getPetTypeIndex(name);
                if (idx >= 0) {
                    petBonus[idx].bonusHp = stoi(hp);
                    petBonus[idx].bonusAttack = stoi(atk);
                    petBonus[idx].bonusSpeed = stof(spd);
                    petBonus[idx].bonusDefense = stoi(def);
                    petBonus[idx].level = stoi(lvl);
                    petBonus[idx].exp = stoi(xp);
                }
            }
            else if (tag == "GUILD") {
                string gname, gw, gl;
                getline(iss, gname, '|');
                getline(iss, gw, '|');
                getline(iss, gl);
                playerGuild.setName(gname);
                playerGuild.setWins(stoi(gw));
                playerGuild.setLosses(stoi(gl));
            }
            else if (tag == "GUILDPET") {
                string pname, prole;
                getline(iss, pname, '|');
                getline(iss, prole);
                playerGuild.addPet(pname, (PetRole)stoi(prole));
            }
        }
        file.close();
        cout << "Game loaded! Welcome back, " << playerName << "!" << endl;
        return true;
    }
    catch (const exception& e) {
        cerr << "Load error: " << e.what() << ". Starting fresh." << endl;
        return false;
    }
}

GameManager::~GameManager() {
    if (window) {
        window->close();
    }
    for (int i = 0; i < inventoryCount; i++) {
        delete inventory[i];
        inventory[i] = nullptr;
    }
    instance = nullptr;
}
