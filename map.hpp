#ifndef CLASS_MAP
#define CLASS_MAP

#include <curses.h>
#include <cstdlib>
#include <vector>

#define MAP_WIDTH 80
#define MAP_HEIGHT 25

#include "maptile.hpp"

class Map
{
private:
    std::vector< std::vector<int> > m_mapArray;

    int m_width;
    int m_height;

public:
    Map(int width, int height);

    void fillMap(int filltile);
    void genDungeon();

    int getWidth() { return m_width;}
    int getHeight() { return m_height;}
    int getMapTile(int x, int y) {return m_mapArray[y][x];}

};
#endif // CLASS_MAP
