#include "item.hpp"

Item::Item(std::string nname, chtype nchar, int ncolor)
{
    //set name
    m_Name = nname;
    //set character
    m_Character = nchar;
    //set color
    m_Color = ncolor;

    //item flags
    m_Walkable = true;
    m_Lightable = true;
}


///////////////////////////////////////////////////
//
ItemInstance::ItemInstance(Item *iref, vector2i ipos)
{
    //link item instance to reference item (master)
    m_ItemRef = iref;
    //set item instances position
    m_Position = ipos;
}

ItemInstance::ItemInstance(Item *iref, int x, int y)
{
    //link item instance to reference item (master)
    m_ItemRef = iref;
    //set item instances position
    m_Position.x = x;
    m_Position.y = y;
}
