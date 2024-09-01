#pragma once
#include "SFML/Graphics.hpp"
#include <math.h>

class Sprite{
private:
    float PI =3.14159;
    std::vector<sf::Vertex> line;
public:
    float speed;
    sf::RectangleShape sprite;
    float posx{100.f}, posy{400.f};

    //player direction x, player direction y, player angle
    float pdx{0.f}, pdy{-1.f}, pa{0.f};
public:
    Sprite();
    ~Sprite();
    void render(sf::RenderTarget& target);
    void move(const float dirx, const float diry);
    //overloaded function
    void move(char x);
    void turn(char x);
    void setPos(sf::Vector2f pos);

    //special functions
    void arenaCollision();

    //accessors
    const sf::Vector2f& getPos() const;
    const sf::FloatRect getBounds() const;
};