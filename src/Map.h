#pragma once
#include <vector>
#include <iostream>

struct Map{
    std::vector<int> map;
    Map(std::string textFile);
    int getTile(int x, int y) const;
    void setTile(int x, int y, int tile);
};