//Muhammad Bilal
//24I-3168
//SE-A


#include "HealthBar.h"
#include <SFML/Graphics.hpp>
using namespace sf;
using namespace std;
void drawHealthBar(RenderWindow& window, Vector2f position, int currentHp, int maxHp, float& animatedHp, Color color, Font& font) {
    // Smooth transition and  linear interpolation
    const float speed = 200.0f; // pixels per second
    if (abs(animatedHp - currentHp) > 0.5f) {
        animatedHp += (currentHp - animatedHp) * 0.1f; // smooth LERP
    }
    else {
        animatedHp = static_cast<float>(currentHp);
    }

    float ratio = max(0.f, animatedHp / maxHp);

    // Draw background
    RectangleShape background(Vector2f(200, 20));
    background.setPosition(position);
    background.setFillColor(Color(80, 80, 80));
    window.draw(background);

    // Draw health
    RectangleShape health(Vector2f(200 * ratio, 20));
    health.setPosition(position);
    health.setFillColor(color);
    window.draw(health);

    // Draw text
    Text hpText;
    hpText.setFont(font);
    hpText.setCharacterSize(14);
    hpText.setFillColor(Color::White);
    hpText.setString(std::to_string(static_cast<int>(animatedHp)) + " / " + to_string(maxHp));
    hpText.setPosition(position.x + 210, position.y - 4); // Offset right of bar
    window.draw(hpText);
}

