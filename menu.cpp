//Muhammad Bilal
//24I-3168
//SE-A



#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Menu.h"
#include "GameManager.h"
#include <iostream>

using namespace std;
using namespace sf;

    Menu::Menu(RenderWindow& win) :
        window(win),
        selectedIndex(0),
        soundEnabled(true),
        selectionMade(false),
        returnValue(-1)
    {
        // Load background
        if (!backgroundTexture.loadFromFile("assets/images/menu.jpg")) {
            throw std::runtime_error("Failed to load background image");
        }
        background.setTexture(backgroundTexture);

        // Scale background to fit window
        float scaleX = (float)window.getSize().x / backgroundTexture.getSize().x;
        float scaleY = (float)window.getSize().y / backgroundTexture.getSize().y;
        background.setScale(scaleX, scaleY);

        // Load resources
        if (!font.loadFromFile("assets/fonts/ariblk.ttf")) {
            throw std::runtime_error("Failed to load font");
        }

        if (!selectBuffer.loadFromFile("assets/audio/hover.wav") ||
            !confirmBuffer.loadFromFile("assets/audio/hover.wav")) {
            throw std::runtime_error("Failed to load sound files");
        }

        selectSound.setBuffer(selectBuffer);
        confirmSound.setBuffer(confirmBuffer);

        // Setup title
        title.setFont(font);
        title.setString("Magical Pets Kingdom");
        title.setCharacterSize(60);
        title.setFillColor(sf::Color(255, 215, 0)); // Gold color
        title.setOutlineColor(sf::Color::Black);
        title.setOutlineThickness(2);
        title.setPosition(
            window.getSize().x / 2 - title.getGlobalBounds().width / 2,
            100
        );

        // Setup menu items
        string items[5] = { "New Game", "Load Game", "Game Modes", "Leaderboard", "Exit" };
        for (int i = 0; i < 5; i++) {
            menuItems[i].setFont(font);
            menuItems[i].setString(items[i]);
            menuItems[i].setCharacterSize(40);
            menuItems[i].setOutlineColor(sf::Color::Black);
            menuItems[i].setOutlineThickness(1);
            menuItems[i].setPosition(
                window.getSize().x / 2 - menuItems[i].getGlobalBounds().width / 2,
                220 + i * 70
            );
        }

        // Setup highlight box
        highlightBox.setSize(sf::Vector2f(350, 50));
        highlightBox.setFillColor(sf::Color(70, 70, 70, 150));
        highlightBox.setOutlineColor(sf::Color(255, 215, 0));
        highlightBox.setOutlineThickness(2);
        updateHighlightPosition();
    }

        void Menu::updateHighlightPosition() {
        highlightBox.setPosition(
            window.getSize().x / 2 - highlightBox.getSize().x / 2,
            215 + selectedIndex * 70
        );
    }

        void Menu::confirmSelection() {
            if (soundEnabled) confirmSound.play();
            selectionMade = true;
            returnValue = selectedIndex;
           
        }

        bool Menu::hasSelection() const {
            return selectionMade;
        }
        int Menu::getSelection() const {
            return returnValue;
        }

        void Menu::reset() {
            selectionMade = false;
            returnValue = -1;
        }


    void Menu::draw() {
        window.clear();

        // Draw background first
        window.draw(background);

        // Draw semi-transparent overlay
        sf::RectangleShape overlay(sf::Vector2f(window.getSize().x, window.getSize().y));
        overlay.setFillColor(sf::Color(0, 0, 0, 150)); // Semi-transparent black
        window.draw(overlay);

        // Draw title
        window.draw(title);

        // Draw highlight
        window.draw(highlightBox);

        // Draw menu items
        for (int i = 0; i < 5; i++) {
            // Highlight selected item
            if (i == selectedIndex) {
                menuItems[i].setFillColor(sf::Color::White);
                menuItems[i].setStyle(sf::Text::Bold);
            }
            else {
                menuItems[i].setFillColor(sf::Color(180, 180, 180));
                menuItems[i].setStyle(sf::Text::Regular);
            }
            window.draw(menuItems[i]);
        }

        window.display();
    }

    void Menu::handleInput() {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                selectionMade = true; // Add this to exit the menu loop
                returnValue = -1;     // Special value indicating window close
                return;
            }

            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                case Keyboard::Up:
                    moveUp();
                    break;
                case Keyboard::Down:
                    moveDown();
                    break;
                case Keyboard::Enter:
                    
                    confirmSelection();
                    break;
                case 4:
                    returnValue = 4; 
                    selectionMade = true;
                    break;
                case Keyboard::S:
                    toggleSound();
                    break;
                default:
                    break;
                }
            }
        }
    }

    void Menu::moveUp() {
        if (soundEnabled) selectSound.play();

        selectedIndex--;
        if (selectedIndex < 0) selectedIndex = 4;
        updateHighlightPosition();
    }

    void Menu::moveDown() {
        if (soundEnabled) selectSound.play();

        selectedIndex++;
        if (selectedIndex > 4) selectedIndex = 0;
        updateHighlightPosition();
    }



   

    void Menu::toggleSound() {
        soundEnabled = !soundEnabled;
        if (soundEnabled) confirmSound.play();
    }


    int Menu::run() {
        while (window.isOpen() &&!selectionMade) {
            handleInput();
            draw();
            
        }
        window.close();
        return returnValue;
    }
