#pragma once
#include "SFML/Graphics.hpp"
#include <vector>

class Renderer{
private:
    std::vector<sf::Uint8> pixel;
    std::vector<sf::Vertex> line;
    sf::Texture texture;
    sf::RectangleShape shape;
public:
    Renderer();
    ~Renderer();
    void renderLine(sf::RenderTarget& target, const sf::Vector2f& begin, 
        const sf::Vector2f& end, sf::Color color);
    void clearPixel();
    void setPixel(int x, int y, sf::Color color);

    //overload function
    void setPixel(int x, int y, sf::Color color, int offset);
    void setPixel(int x, int y, int width, int height, sf::Color color);
    void render(sf::RenderTarget& target);
};