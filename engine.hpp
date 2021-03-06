#include <cstdlib>
#include <curses.h>


#include "map.hpp"
#include "player.hpp"
#include "astar.hpp"

enum {D_NA, D_SOUTHWEST, D_SOUTH, D_SOUTHEAST, D_WEST, D_NODIR, D_EAST, D_NORTHWEST, D_NORTH, D_NORTHEAST, D_TOTAL};

struct Message
{
    std::string msg;
    int life;
};

class Engine
{
private:

    //init
    void initCurses();
    void initColors();
    void initTiles();
    void initItems();
    void initMobs();

    void newGame(int nseed = -1);
    void initMap();
    void genMap();
    void initPlayer();

    //map generation
    void mapDrawBox(Map *tmap, int x1, int y1, int x2, int y2, int tile);
    int genGrass();


    //loops
    void mainLoop();

    //draw
    void drawPlayer(int x, int y);
    void drawMap(vector2i maptopleftpos, recti mapwindow);
    void drawUI(int x, int y);
    bool inFov(int sx, int sy, int tx, int ty);
    bool posLightable(int x, int y);
    recti m_MapWindow;

    //objects
    int m_Seed;
    std::vector<MapTile> m_Tiles;
    std::vector<Item*> m_Items;
    Item* findItemByName(std::string iname); //item helper function
    ItemInstance *createItemInstanceFromItem(Item *titem);
    std::vector<Mob> m_Mobs;
    std::vector<Map*> m_Maps;
    Map *m_currentMap;
    Player *m_Player;


    //mob/player actions
    vector2i moveDirection(vector2i startpos, int dir);
    void PlayerGetItem();
    void PlayerDropItem();
    void PlayerInventory();
    void actionClose();
    void doMobTurn();
    //this returns a chunk of the map to pass to astar for pathfinding
    //give it a map, a radius to copy, starting position , and target position
    std::vector<std::vector<int> > getAStarMapChunk(Map *tmap, int radius, int sx, int sy, int tx, int ty);

    //messaging
    std::vector<Message> m_MessageQue;
    void addMessage(std::string amsg);
    void drawMessages();

    //debug
    void d_debugmenu();
    bool d_light;
    void d_showmapitems();

public:
    Engine();

    void start();
    int getSeed() { return m_Seed;}
};
