#include "Sprite.h"
#include "Constants.h"

Sprite::Sprite(){
    speed = 3.f;
    sprite.setSize(sf::Vector2f(10.f,10.f));
    sprite.setFillColor(sf::Color::Red);
    sprite.setPosition(posx,posy);
}

Sprite::~Sprite(){
    std::cout << "Destroyed Sprite!" << std::endl;
}

void Sprite::render(sf::RenderTarget& target){
    target.draw(sprite);
}

void Sprite::move(char x){
    switch (x){
        case 'f':
            sprite.move(pdx*speed,pdy*speed);
            break;
        case 'b':
            sprite.move(-pdx*speed,-pdy*speed);
            break;
    }
}

void Sprite::turn(char x){
    switch (x){
        case 'l':
            pa -= speed/100.f;
            if (pa<0){ pa +=2*PI; }
            pdx = std::cos(pa);
            pdy = std::sin(pa);
            break;
        case 'r':
            pa += speed/100.f;
            if (pa>2*PI){ pa -=2*PI; }
            pdx = std::cos(pa);
            pdy = std::sin(pa);
            break;
    }
}

void Sprite::setPos(sf::Vector2f pos){
    sprite.setPosition(pos);
}

const sf::Vector2f& Sprite::getPos() const{
    return sprite.getPosition();
}

const sf::FloatRect Sprite::getBounds() const{
    return sprite.getGlobalBounds();
}