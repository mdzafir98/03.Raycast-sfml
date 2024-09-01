#include "Renderer.h"
#include "Constants.h"
#include <cstring>

Renderer::Renderer() 
    : pixel(WINDOW_WIDTH*(WINDOW_HEIGHT)*4){
    line.emplace_back(sf::Vector2f{0.f,0.f}, sf::Color::Red);
    line.emplace_back(sf::Vector2f{0.f,0.f}, sf::Color::Red);
    texture.create(WINDOW_WIDTH, WINDOW_HEIGHT);
    shape.setSize({WINDOW_WIDTH, WINDOW_HEIGHT});
}

Renderer::~Renderer(){
    std::cout << "Destroyed Renderer!" << std::endl;
}

void Renderer::renderLine(sf::RenderTarget& target, const sf::Vector2f& begin, 
    const sf::Vector2f& end, sf::Color color){
    line[0].position = begin;
    line[1].position = end;
    line[0].color = color;
    line[1].color = color;
    target.draw(line.data(),2,sf::PrimitiveType::Lines);
}

void Renderer::clearPixel(){
    std::memset(pixel.data(),0,pixel.size());
}

void Renderer::setPixel(int x, int y, sf::Color color){
    if (x<0 || x>= WINDOW_WIDTH || y<0 || y>= WINDOW_HEIGHT){
        return;
    }

    sf::Uint8* ptr = &pixel.at((y*(WINDOW_WIDTH/2)+x)*4);
    ptr[0] =color.r;
    ptr[1] =color.g;
    ptr[2] =color.b;
    ptr[3] =255;
}

void Renderer::setPixel(int x, int y, sf::Color color, int offset){
    if (x<0 || x>= WINDOW_WIDTH || y<0 || y>= WINDOW_HEIGHT){
        return;
    }

    sf::Uint8* ptr = &pixel.at((y*(WINDOW_WIDTH)+x+offset)*4);
    ptr[0] =color.r;
    ptr[1] =color.g;
    ptr[2] =color.b;
    ptr[3] =255;
}

void Renderer::setPixel(int x, int y, int width, int height, sf::Color color){
    
}

void Renderer::render(sf::RenderTarget& target){
    shape.setTexture(&texture);
    texture.update(pixel.data());
    target.draw(shape);
}