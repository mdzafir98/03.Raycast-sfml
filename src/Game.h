#pragma once
#include <memory>
#include <vector>

#include "Constants.h"
#include "Sprite.h"
#include "Renderer.h"

#include "SFML/Graphics.hpp"

class Game{
private:
//variables for raycast function
    int r, mx, my, mp, dof;
    float rx{0}, ry {0}, ra, xo{0}, yo{0};

    sf::RenderWindow* window;
    Sprite* player;
    Renderer* renderer;
    sf::CircleShape ball;
    bool canMove =true;

    void initWindow();
    void initPlayer();
    void initMinimapTile();
    void initRenderer();
    void initMouse();
private:
    //important math functions and variables
    int bitSize =8;
    int mapx =bitSize, mapy =bitSize;
    int mapS =bitSize*bitSize;

    int map2D[64] =
{
	1,1,1,1,1,1,1,1,
	1,0,2,0,0,0,0,1,
	1,0,2,0,2,0,0,1,
	1,0,0,0,0,0,0,1,
	1,0,0,0,0,2,0,1,
	1,0,2,0,0,2,0,1,
	1,0,0,0,0,0,0,1,
	1,1,1,1,1,1,1,1
};

    float rads(float deg){
        return deg*PI/180.f;
    }

    float wrap(float angle){
        if (angle<0){
            return angle + 360.f;
        }
        else if (angle>360.f){
            return angle-360.f;
        }
        else {
            return angle;
        }
    }

    float dist(const sf::Vector2f& vec1, sf::Vector2f& vec2){
        float dx = (vec1.x-vec2.x);
        float dy = (vec1.y-vec2.y);
        return std::sqrt((dx*dx)+(dy*dy));
    }

    //overload dist function
    float dist(float ax, float ay, float bx, float by){
        return std::sqrt((bx - ax) * (bx - ax) 
            + (by - ay) * (by - ay));
    }
private:
    //for map and tile variables
    sf::RectangleShape minimapTile;
    struct Tile{
        int tileType;
        sf::RectangleShape tileShape;
        const sf::FloatRect getBounds() const{
            return tileShape.getGlobalBounds();
        }
    };
    Tile wall;
    std::vector<sf::RectangleShape> obstacles;
    void updateCollision();
    void setObstacle();
    void renderObstacle();
public:
    Game();
    ~Game();

    void run();

    //update
    void raycast();
    void updatePollevents();
    void updateInput();
    void update();

    //render
    void render();
    void renderMap();
    void renderMap2();
    void renderRaycast();
    void renderPlayerLine();
};