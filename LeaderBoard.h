#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <fstream>
using namespace std;
using namespace sf;

class LeaderBoard {
public:
    LeaderBoard();
    ~LeaderBoard();

    void createWindow();
    void run();
    void render();

    // Dynamic leaderboard
    void addResult(const string& playerName, bool won);
    void saveToFile();
    void loadFromFile();
    void sortByWins();

private:
    RenderWindow* window;
    Font font;
    Texture backgroundTexture;
    Sprite background;

    static const int MAX_ENTRIES = 10;
    string playerNames[MAX_ENTRIES];
    int wins[MAX_ENTRIES];
    int losses[MAX_ENTRIES];
    int entryCount;
};