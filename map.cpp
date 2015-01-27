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

void Map::genDungeon()
{
    int temptile = 1;

    for(int i = 0; i < 20; i++)
    {
        int rx = rand()%m_width;
        int ry = rand()%m_height;

        m_mapArray[ry][rx] = temptile;
    }

    //set dome markers for debug
    int debugtile = 1;

    //add border around map
    for(int i = 0; i < m_height; i++)
    {
        for(int n = 0; n < m_width; n++)
        {
            if(i == 0 || i == m_height-1)
            {
                m_mapArray[i][n] = debugtile;
            }
            else if(n == 0 || n == m_width-1)
            {
                m_mapArray[i][n] = debugtile;
            }
        }
    }

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
