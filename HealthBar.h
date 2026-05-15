//Muhammad Bilal
//24I-3168
//SE-A
#ifndef HealthBar_h
#define HealthBar_h
// header file for healthbar

#include <SFML/Graphics.hpp>
using namespace sf;
void drawHealthBar(RenderWindow& window, Vector2f position, int currentHp, int maxHp, float& animatedHp, Color color, Font& font);
#endif