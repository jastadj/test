#ifndef CLASS_MAP
#define CLASS_MAP

#include <curses.h>
#include <cstdlib>
#include <vector>

#define MAP_WIDTH 80
#define MAP_HEIGHT 25

#include "maptile.hpp"
#include "item.hpp"
#include "mob.hpp"

class Map
{
private:
    std::vector< std::vector<int> > m_mapArray;
    std::vector<ItemInstance*> m_Items;
    std::vector<MobInstance*> m_Mobs;

    int m_width;
    int m_height;

public:
    Map(int width, int height);
    ~Map();

    void fillMap(int filltile);

    int getWidth() { return m_width;}
    int getHeight() { return m_height;}

    int getMapTile(int x, int y) {return m_mapArray[y][x];}
    void setMapTile(int x, int y, int tile);

    //items
    void addItem(ItemInstance *nitem);
    void removeItem(ItemInstance *ritem);
    std::vector<ItemInstance*> getItemsAt(int x, int y);

    void addMob(MobInstance *nmob);
    void removeMob(MobInstance *nmob);
    MobInstance *getMobAt(int x, int y);
    std::vector<MobInstance*> *getMobs() { return &m_Mobs;}


    std::vector<ItemInstance*> *d_getmapitems() { return &m_Items;}

};
#endif // CLASS_MAP
