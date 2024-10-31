#include "Game.h"

void Game::initWindow(){
    window =new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH,WINDOW_HEIGHT),"Raycast!", 
        sf::Style::Close | sf::Style::Titlebar);
    window->setFramerateLimit(60);
    window->setVerticalSyncEnabled(false);
}

void Game::initPlayer(){
    player = new Sprite();
}

void Game::initMinimapTile(){
    minimapTile.setSize({64,64});
    minimapTile.setOutlineThickness(1.f);
    wall.tileShape.setSize({64,64});
    wall.tileShape.setOutlineThickness(1.f);
}

void Game::initRenderer(){
    renderer = new Renderer();
}

void Game::initMouse(){
    sf::Vector2i mousePosWindow = sf::Mouse::getPosition(*window);
    sf::Vector2f mousePosView = window->mapPixelToCoords(mousePosWindow);
}

Game::Game(){
    initWindow();
    initPlayer();
    initMinimapTile();
    setObstacle();
    initRenderer();
}

Game::~Game(){
    delete window;
    delete player;
    delete renderer;
    std::cout << "Game is destroyed!" 
        << std::endl;
}

void Game::run(){
    while (window->isOpen()){
        updatePollevents();
        update();
        render();
    }
}

void Game::raycast(){
    //spriteCentre is the centre of the sprite
    sf::Vector2f spriteCentre(player->getPos().x + player->getBounds().width/2, 
        player->getPos().y + player->getBounds().height/2);
    ra =player->pa - dr2*160; //the 160/2 is to offset the number of rays
    if (ra<0){ra+=2*PI;}
    if (ra>2*PI){ra-=2*PI;}
    for (r=0; r<320; r++){
        float distH =1000000;
        float distV =1000000;
        float sDist =0;
        float hx =spriteCentre.x, hy =spriteCentre.y;
        float vx =spriteCentre.x, vy =spriteCentre.y;

        //check for horizontal line collisions
        {
            dof =0;
            float aTan =-1/std::tan(ra); //inverse tan of player angle
            //if ray is looking down
            if (ra>PI){
                ry =std::floor(spriteCentre.y/64)*64-0.001;
                rx =(spriteCentre.y-ry)*aTan + spriteCentre.x;
                yo =-64;
                xo =-yo*aTan;
            }
            //looking up
            if (ra<PI){
                ry =std::floor(spriteCentre.y/64)*64 + 64;
                rx =(spriteCentre.y-ry)*aTan + spriteCentre.x;
                yo =64;
                xo =-yo*aTan;
            }
            //if ray is looking at horizontal line
            if (ra==0.f || ra==PI){
                rx =spriteCentre.x;
                ry =spriteCentre.y;
                dof =bitSize;
            }
            while (dof<bitSize){
                mx =(rx/64);
                my =(ry/64);
                mp = my*bitSize + mx;

                //check for wall collisions to stop the ray
                if (mp>0 && mp <mapx*mapy && (map2D[mp]==1 or map2D[mp]==2)){
                    hx =rx;
                    hy =ry;
                    distH =dist(spriteCentre.x, spriteCentre.y,rx,ry);
                    dof =bitSize;
                }
                else{
                    rx +=xo;
                    ry +=yo;
                    dof +=1;
                }
            }
        }

        //check for vertical line collisions
        {
            dof =0;
            float nTan =-std::tan(ra);
            if (ra>PI2 && ra<PI3){
                rx =std::floor(spriteCentre.x/64)*64 - 0.001;
                ry =(spriteCentre.x-rx)*nTan + spriteCentre.y;
                xo =-64;
                yo =-xo*nTan;
            }
            if (ra<PI2 || ra>PI3){
                rx = std::floor(spriteCentre.x/64)*64 + 64;
                ry =(spriteCentre.x-rx)*nTan + spriteCentre.y;
                xo =64;
                yo =-xo*nTan;
            }
            //if ray is looking horizontal line
            if (ra==0.f || ra==PI){
                rx =spriteCentre.x;
                ry =spriteCentre.y;
                dof = bitSize;
            }
            while (dof<bitSize){
                mx =(rx/64);
                my =(ry/64);
                mp = my*bitSize + mx;

                //check for wall collision to stop the ray
                if (mp>0 && mp<mapx*mapy && (map2D[mp]==1 or map2D[mp]==2)){
                    vx =rx;
                    vy =ry;
                    distV =dist(spriteCentre.x, spriteCentre.y, vx, vy);
                    dof =bitSize;
                }
                else{
                    rx +=xo;
                    ry +=yo;
                    dof +=1;
                }
            }
        }
        //the ray with the shortest distance is drawn on the screen
        sf::Color wallColor;
        uint8_t colorCode =255;
        if (distH<distV){
            renderer->renderLine(*window,{spriteCentre.x,spriteCentre.y}, 
                {hx,hy}, {255,0,0});
            sDist = distH;
            wallColor = { static_cast<uint8_t>(colorCode / 2),0,0 };
        }
        if (distV<distH){
            renderer->renderLine(*window, {spriteCentre.x,spriteCentre.y},
                {vx,vy},{255,0,0});
            sDist =distV;
            wallColor ={colorCode,0,0};
        }
        //draw the 3d walls here
        sDist =std::cos(rads(120/2))*sDist;
        float wallHeight =64/sDist*(WINDOW_HEIGHT/2/std::tan(rads(120/2)));
        int start =(WINDOW_HEIGHT/2) - wallHeight/2;
        int offset =WINDOW_WIDTH/2 + 80;
        for (int y =start; y<start + wallHeight; y++){
            renderer->setPixel(r,y,wallColor,offset);
        }
        ra += dr2;
        if (ra<0){
            ra += 2*PI;
        }
        if (ra>2*PI){
            ra -= 2*PI;
        }
    }
}

void Game::updatePollevents(){
    sf::Event e;
    while (window->pollEvent(e)){
        if (e.type ==sf::Event::Closed){
            window->close();
        }
        if (e.type ==sf::Event::KeyPressed && e.key.code ==sf::Keyboard::Escape){
            window->close();
        }
    }
}

void Game::updateInput(){
    //move player using wasd
    if (canMove){
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
            player->move('f');
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
            player->turn('l');
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
            player->move('b');
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
            player->turn('r');
        }
    }
}

void Game::update(){
    sf::Time elapsed2 = clock.getElapsedTime();
    double currentTime = elapsed2.asSeconds();
    if (currentTime-lastTime>1.0f/150.f){
        updateCollision();
        updateInput();
    }
    // updateInput();
    // updateCollision();
}

void Game::render(){
    window->clear(sf::Color::Black);
    renderMap();
    renderRaycast();
    renderPlayerLine();
    renderObstacle();
    player->render(*window);
    window->display();
}

void Game::renderMap2(){
    for (int y=0; y<mapy; y++){
        for (int x=0; x<mapx; x++){
            switch(map2D[y*bitSize+x]){
                case 1:
                    minimapTile.setFillColor(sf::Color(255,255,255,255));
                    minimapTile.setOutlineColor(sf::Color::Black);
                    break;
                default:
                    minimapTile.setFillColor(sf::Color(0,0,0,255));
                    minimapTile.setOutlineColor(sf::Color::White);
                    break;
            }
            minimapTile.setPosition(x*mapS, y*mapS);
            window->draw(minimapTile);
        }
    }
}

void Game::renderMap(){
    for (int y=0; y<mapy; y++){
        for (int x=0; x<mapx; x++){
            switch(map2D[y*bitSize+x]){
                case 1:
                    wall.tileType =1;
                    wall.tileShape.setFillColor(sf::Color(255,255,255,255));
                    wall.tileShape.setOutlineColor(sf::Color::Black);
                    break;
                case 2:
                    wall.tileType =2;
                    wall.tileShape.setFillColor(sf::Color(255,255,255,255));
                    wall.tileShape.setOutlineColor(sf::Color::Black);
                    break;
                default:
                    wall.tileType =0;
                    wall.tileShape.setFillColor(sf::Color(0,0,0,255));
                    wall.tileShape.setOutlineColor(sf::Color::White);
                    break;
            }
            wall.tileShape.setPosition(x*mapS, y*mapS);
            window->draw(wall.tileShape);
        }
    }
}

void Game::setObstacle(){
    for (int y=0; y<mapy; y++){
        for (int x =0; x<mapx; x++){
            if (map2D[y*bitSize+x]==2){
                minimapTile.setPosition(x*mapS, y*mapS);
                minimapTile.setFillColor({128,0,0,255});
                minimapTile.setOutlineColor({128,0,0,255});
                obstacles.push_back(minimapTile);
            }
        }
    }
}

void Game::renderObstacle(){
    for (auto i:obstacles){
        window->draw(i);
    }
}

void Game::renderRaycast(){
    renderer->clearPixel();
    raycast();
    renderer->render(*window);
}

void Game::renderPlayerLine(){
    sf::Vector2f spriteCentre(player->getPos().x + player->getBounds().width/2, 
        player->getPos().y + player->getBounds().height/2);
    renderer->renderLine(*window, {spriteCentre.x,spriteCentre.y},
        {spriteCentre.x+player->pdx*20,spriteCentre.y+player->pdy*20},{255,255,255});
}

void Game::updateCollision()
{
    //collision with bottom wall
    if (player->getBounds().top+player->getBounds().height>window->getSize().y-64.f){
        std::cout << "collision with bottom wall!" << " ";
        std::cout << "player position: " << player->getPos().x << " " << player->getPos().y << "\n";
        player->setPos({player->getBounds().left,window->getSize().y-player->getBounds().height-64.f});
    }
    //collision with left wall
    if (player->getBounds().left<64.f){
        std::cout << "collision with left wall!" << " ";
        std::cout << "player position: " << player->getPos().x << " " << player->getPos().y << "\n";
        player->setPos({64.f,player->getBounds().top});
    }
    //collision with right wall
    if (player->getBounds().left+player->getBounds().width>window->getSize().x/2-64.f){
        std::cout << "collision with right wall!" << " ";
        std::cout << "player position: " << player->getPos().x << " " << player->getPos().y << "\n";
        player->setPos({window->getSize().x/2-player->getBounds().width-64.f,player->getBounds().top});
    }
    //collision with top wall
    if (player->getBounds().top<64.f){
        std::cout << "collision with top wall!" << " ";
        std::cout << "player position: " << player->getPos().x << " " << player->getPos().y << "\n";
        player->setPos({player->getBounds().left,64.f});
    }
    //TODO: collision with obstacles
    for (auto obstacle:obstacles){
        if (player->getBounds().intersects(obstacle.getGlobalBounds())){
            canMove =false;
            std::cout << "collision with obstacle! player position: " << player->getPos().x << " " << player->getPos().y <<"\n";
            player->setPos({player->getPos().x+0.001f,player->getPos().y+0.001f});
        }
        canMove =true;
    }
}