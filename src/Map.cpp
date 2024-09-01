#include "Map.h"
#include "Constants.h"
#include <fstream>
#include <cassert>
#include <sstream>

Map::Map(std::string textFile){
    std::ifstream inFile(textFile);
    std::string line;
    map.reserve(MAP_SIZE*MAP_SIZE);
    while(std::getline(inFile,line)){
        for (auto c:line){
            map.push_back(c-'0');
        }
    }
    assert(map.size()==(MAP_SIZE*MAP_SIZE));
}

int Map::getTile(int x, int y) const{
    if (x<0 || x>MAP_SIZE || y<0 || y>MAP_SIZE){
        return 0;
    }
    return map[y*MAP_SIZE + x];
}

void Map::setTile(int x, int y, int tile){
    if (x<0 || x>MAP_SIZE || y<0 || y>MAP_SIZE){
        return;
    }
    map[y*MAP_SIZE+x] = tile;
}