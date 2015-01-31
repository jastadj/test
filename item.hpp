#ifndef CLASS_ITEM
#define CLASS_ITEM

#include <curses.h>
#include <string>
#include "common.hpp"

class Item
{
private:
    std::string m_Name;
    chtype m_Character;
    int m_Color;

    bool m_Walkable;
    bool m_Lightable;

public:
    Item(std::string nname, chtype nchar, int ncolor);

    std::string getName() { return m_Name;}
    int getColor() { return m_Color;}
    chtype getCharacter() { return  m_Character;}

    void setWalkable(bool npass) { m_Walkable = npass;}
    bool isWalkable() { return m_Walkable;}
    void setLightable(bool nlight) { m_Lightable = nlight;}
    bool isLightable() { return m_Lightable;}
};

class ItemInstance
{
private:
    Item *m_ItemRef;

    vector2i m_Position;

public:
    ItemInstance(Item *iref, vector2i ipos);
    ItemInstance(Item *iref, int x, int y);

    vector2i getPosition() { return m_Position;}
    void setPosition(vector2i newpos) { m_Position = newpos;}
    void setPosition(int x, int y) {m_Position.x = x; m_Position.y = y;}

    std::string getName() { return m_ItemRef->getName();}
    int getColor() {return m_ItemRef->getColor();}
    chtype getCharacter() { return m_ItemRef->getCharacter();}
    bool isWalkable() { return m_ItemRef->isWalkable();}
    bool isLightable() { return m_ItemRef->isLightable();}


};
#endif // CLASS_ITEM
