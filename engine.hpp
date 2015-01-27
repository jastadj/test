#include <cstdlib>
#include <curses.h>


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
    void initItems();

    void newGame(int nseed = -1);
    void initMap();
    void initPlayer();

    //loops
    void mainLoop();

    //draw
    void drawPlayer(int x, int y);
    void drawMap(vector2i maptopleftpos, recti mapwindow);
    void drawUI(int x, int y);
    bool inFov(int sx, int sy, int tx, int ty);
    recti m_MapWindow;

    //objects
    int m_Seed;
    std::vector<MapTile> m_Tiles;
    std::vector<Item> m_Items;
    std::vector<Map*> m_Maps;
    Map *m_currentMap;
    Player *m_Player;

    //mob/player actions
    vector2i moveDirection(vector2i startpos, int dir);
    void PlayerGetItem();
    void PlayerDropItem();
    void PlayerInventory();

    //debug
    void d_debugmenu();
    bool d_light;
    void d_showmapitems();

public:
    Engine();

    void start();
};
