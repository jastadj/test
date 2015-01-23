#include <cstdlib>
#include <iostream>
#include <string>
#include <curses.h>
#include <time.h>

#include "map.hpp"
#include "player.hpp"

enum {D_NA, D_SOUTHWEST, D_SOUTH, D_SOUTHEAST, D_WEST, D_NODIR, D_EAST, D_NORTHWEST, D_NORTH, D_NORTHEAST, D_TOTAL};

class Engine
{
private:

    //init
    void initCurses();
    void initColors();
    void initTiles();

    void newGame();
    void initMap();
    void initPlayer();

    //loops
    void mainLoop();

    //draw
    void drawPlayer(int x, int y);
    void drawMap(vector2i maptopleftpos, recti mapwindow);
    bool inFov(int x, int y);
    recti m_MapWindow;

    //objects
    std::vector<MapTile> m_Tiles;
    std::vector<Map> m_Maps;
    Map *m_currentMap;
    Player *m_Player;

    //
    vector2i moveDirection(vector2i startpos, int dir);

public:
    Engine();

    void start();
};
