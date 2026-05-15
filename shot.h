//Muhammad Bilal
//24I-3168
//SE-A

#ifndef shot_h
#define shot_h

#include <SFML/Graphics.hpp>
using namespace sf;

class Shot {
private:
    CircleShape shape;
    Vector2f velocity;
    bool fromPlayer;
    bool active;

public:
    Shot(Vector2f pos, Vector2f vel, bool isFromPlayer);

    Vector2f getPosition() const;
    void update();

    bool checkCollision(const Sprite& target);

    void deactivate();
    bool isActive() const;

    bool isFromPlayer() const;

    void draw(RenderWindow& window) const;
};

#endif 


