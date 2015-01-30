#include "map.hpp"

Map::Map(int width, int height)
{
    m_width = width;
    m_height = height;

    //resize map array to width/height
    m_mapArray.resize(m_height);
    for(int i = 0; i < m_height; i++) m_mapArray[i].resize(m_width);

    //clear map data
    fillMap(0);

}

Map::~Map()
{
    for(int i = int(m_Items.size()-1); i >= 0; i--)
    {
        delete m_Items[i];
    }

    m_Items.clear();
}

void Map::fillMap(int fillTile)
{
    for(int i = 0; i < m_height; i++)
    {
        for(int n = 0; n < m_width; n++)
        {
            m_mapArray[i][n] = fillTile;
        }
    }
}

void Map::setMapTile(int x, int y, int tile)
{
    if(x < 0 || x >= m_width || y < 0 || y >= m_height) return;

    m_mapArray[y][x] = tile;
}

void Map::addItem(ItemInstance *nitem)
{
    m_Items.push_back(nitem);
}

void Map::removeItem(ItemInstance *ritem)
{
    for(int i = int(m_Items.size()-1); i >= 0; i--)
    {
        if(ritem == m_Items[i])
        {
            m_Items.erase(m_Items.begin() + i);
            return;
        }
    }
}

std::vector<ItemInstance*> Map::getItemsAt(int x, int y)
{
    std::vector<ItemInstance*> founditems;

    for(int i = 0; i < int(m_Items.size()); i++)
    {
        vector2i ipos = m_Items[i]->getPosition();

        if(ipos.x == x && ipos.y == y) founditems.push_back(m_Items[i]);
    }

    return founditems;
}

//////////////////////////////////////////////////////////////////////////
//

void Map::addMob(MobInstance *nmob)
{
    m_Mobs.push_back(nmob);
}

void Map::removeMob(MobInstance *rmob)
{
    for(int i = int(m_Mobs.size()-1); i >= 0; i--)
    {
        if(rmob == m_Mobs[i])
        {
            m_Mobs.erase(m_Mobs.begin() + i);
            return;
        }
    }
}

MobInstance *Map::getMobAt(int x, int y)
{

    for(int i = 0; i < int(m_Mobs.size()); i++)
    {
        vector2i ipos = m_Mobs[i]->getPosition();

        if(ipos.x == x && ipos.y == y) return m_Mobs[i];
    }

    return NULL;
}
