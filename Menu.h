//Muhammad Bilal
//24I-3168
//SE-A



#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "GameManager.h"
#include <iostream>
using namespace std;
using namespace sf;

class Menu {
private:
    RenderWindow& window;
    Font font;
    GameManager* manage;
    // Background
    Texture backgroundTexture;
    Sprite background;

    // Menu items
    Text title;
    Text menuItems[5];
    RectangleShape highlightBox;

    // Sound
    SoundBuffer selectBuffer;
    SoundBuffer confirmBuffer;
    Sound selectSound;
    Sound confirmSound;

    int selectedIndex;
    bool soundEnabled;
    bool selectionMade;
    int returnValue;

public:
    Menu(RenderWindow& win);

    void updateHighlightPosition();
    void reset();
    int getSelection() const;
    bool hasSelection() const;
    void confirmSelection();


    void draw();

    void handleInput();
    void moveUp();
    void moveDown();


    

    void toggleSound();


    int run();
};