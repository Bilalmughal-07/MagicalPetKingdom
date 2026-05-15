#include "Leaderboard.h"
#include <iostream>
#include <fstream>
#include <sstream>

LeaderBoard::LeaderBoard() : window(nullptr), entryCount(0) {
    if (!font.loadFromFile("assets/fonts/ariblk.ttf")) {
        cerr << "Failed to load font!" << endl;
    }
    if (!backgroundTexture.loadFromFile("assets/images/lb_bg.jpg")) {
        backgroundTexture.create(1280, 720);
        Uint8* pixels = new Uint8[1280 * 720 * 4];
        for (int i = 0; i < 1280 * 720 * 4; i += 4) {
            pixels[i] = 30; pixels[i+1] = 30; pixels[i+2] = 60; pixels[i+3] = 255;
        }
        backgroundTexture.update(pixels);
        delete[] pixels;
    }
    background.setTexture(backgroundTexture);
    for (int i = 0; i < MAX_ENTRIES; i++) {
        playerNames[i] = "";
        wins[i] = 0;
        losses[i] = 0;
    }
    loadFromFile();
}

LeaderBoard::~LeaderBoard() {
    if (window) delete window;
}

void LeaderBoard::createWindow() {
    if (!window) {
        window = new RenderWindow(VideoMode(1280, 720), "Leaderboard");
    }
}

void LeaderBoard::addResult(const string& playerName, bool won) {
    if (playerName.empty()) return;
    // Find existing entry
    for (int i = 0; i < entryCount; i++) {
        if (playerNames[i] == playerName) {
            if (won) wins[i]++;
            else losses[i]++;
            sortByWins();
            saveToFile();
            return;
        }
    }
    // Add new entry
    if (entryCount < MAX_ENTRIES) {
        playerNames[entryCount] = playerName;
        wins[entryCount] = won ? 1 : 0;
        losses[entryCount] = won ? 0 : 1;
        entryCount++;
        sortByWins();
        saveToFile();
    }
}

void LeaderBoard::sortByWins() {
    // Simple bubble sort by wins (descending)
    for (int i = 0; i < entryCount - 1; i++) {
        for (int j = 0; j < entryCount - i - 1; j++) {
            if (wins[j] < wins[j + 1]) {
                swap(playerNames[j], playerNames[j + 1]);
                swap(wins[j], wins[j + 1]);
                swap(losses[j], losses[j + 1]);
            }
        }
    }
}

void LeaderBoard::saveToFile() {
    try {
        ofstream file("leaderboard.txt");
        if (!file.is_open()) return;
        file << entryCount << endl;
        for (int i = 0; i < entryCount; i++) {
            file << playerNames[i] << "|" << wins[i] << "|" << losses[i] << endl;
        }
        file.close();
    } catch (...) {
        cerr << "Failed to save leaderboard." << endl;
    }
}

void LeaderBoard::loadFromFile() {
    try {
        ifstream file("leaderboard.txt");
        if (!file.is_open()) return;
        string line;
        getline(file, line);
        entryCount = stoi(line);
        if (entryCount > MAX_ENTRIES) entryCount = MAX_ENTRIES;
        for (int i = 0; i < entryCount; i++) {
            getline(file, line);
            istringstream iss(line);
            string name, w, l;
            getline(iss, name, '|');
            getline(iss, w, '|');
            getline(iss, l);
            playerNames[i] = name;
            wins[i] = stoi(w);
            losses[i] = stoi(l);
        }
        file.close();
        sortByWins();
    } catch (...) {
        entryCount = 0;
    }
}

void LeaderBoard::run() {
    if (!window) { cerr << "Window not created!" << endl; return; }
    while (window->isOpen()) {
        Event event;
        while (window->pollEvent(event)) {
            if (event.type == Event::Closed ||
                (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)) {
                window->close();
            }
        }
        window->clear();
        render();
        window->display();
    }
    delete window;
    window = nullptr;
}

void LeaderBoard::render() {
    if (!window) return;
    float sx = (float)window->getSize().x / backgroundTexture.getSize().x;
    float sy = (float)window->getSize().y / backgroundTexture.getSize().y;
    background.setScale(sx, sy);
    window->draw(background);

    // Overlay
    RectangleShape overlay(Vector2f(1280, 720));
    overlay.setFillColor(Color(0, 0, 0, 150));
    window->draw(overlay);

    // Title
    Text title("LEADERBOARD", font, 48);
    title.setFillColor(Color(255, 215, 0));
    title.setPosition(window->getSize().x / 2.f - title.getGlobalBounds().width / 2, 20);
    window->draw(title);

    // Column headers
    string hdrStr[] = { "Rank", "Player", "Wins", "Losses", "W/L Ratio" };
    float hdrX[] = { 150, 280, 520, 670, 830 };
    for (int i = 0; i < 5; i++) {
        Text h(hdrStr[i], font, 24);
        h.setFillColor(Color::Yellow);
        h.setPosition(hdrX[i], 90);
        window->draw(h);
    }

    // Separator
    RectangleShape sep(Vector2f(800, 2));
    sep.setPosition(150, 125);
    sep.setFillColor(Color(255, 215, 0, 120));
    window->draw(sep);

    // Entries
    if (entryCount == 0) {
        Text empty("No battles recorded yet. Go fight!", font, 24);
        empty.setFillColor(Color(200, 200, 200));
        empty.setPosition(350, 300);
        window->draw(empty);
    }
    for (int i = 0; i < entryCount; i++) {
        Color rowColor = (i == 0) ? Color(255, 215, 0) :
                         (i == 1) ? Color(200, 200, 200) :
                         (i == 2) ? Color(205, 127, 50) : Color::White;

        // Highlight bar for top 3
        if (i < 3) {
            RectangleShape bar(Vector2f(780, 38));
            bar.setPosition(145, 133 + i * 45);
            bar.setFillColor(Color(rowColor.r, rowColor.g, rowColor.b, 25));
            window->draw(bar);
        }

        Text rank("#" + to_string(i + 1), font, 22);
        rank.setFillColor(rowColor);
        rank.setPosition(160, 138 + i * 45);
        window->draw(rank);

        Text name(playerNames[i], font, 22);
        name.setFillColor(rowColor);
        name.setPosition(280, 138 + i * 45);
        window->draw(name);

        Text w(to_string(wins[i]), font, 22);
        w.setFillColor(Color(100, 255, 100));
        w.setPosition(540, 138 + i * 45);
        window->draw(w);

        Text l(to_string(losses[i]), font, 22);
        l.setFillColor(Color(255, 100, 100));
        l.setPosition(690, 138 + i * 45);
        window->draw(l);

        int total = wins[i] + losses[i];
        string ratio = total > 0 ? to_string(wins[i] * 100 / total) + "%" : "N/A";
        Text r(ratio, font, 22);
        r.setFillColor(Color::White);
        r.setPosition(850, 138 + i * 45);
        window->draw(r);
    }

    Text footer("Press ESC to return", font, 18);
    footer.setFillColor(Color(180, 180, 180));
    footer.setPosition(window->getSize().x / 2.f - footer.getGlobalBounds().width / 2, window->getSize().y - 40.f);
    window->draw(footer);
}