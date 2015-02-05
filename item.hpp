#ifndef CLASS_ITEM
#define CLASS_ITEM

#include <curses.h>
#include <string>
#include "common.hpp"

enum {I_GENERIC, I_DOOR};

class Item
{
protected:
    std::string m_Name;
    chtype m_Character;
    int m_Color;

    bool m_Walkable;
    bool m_Lightable;

public:
    Item(std::string nname, chtype nchar, int ncolor);
    virtual int getType() = 0;

    std::string getName() { return m_Name;}
    int getColor() { return m_Color;}
    chtype getCharacter() { return  m_Character;}

    void setWalkable(bool npass) { m_Walkable = npass;}
    bool isWalkable() { return m_Walkable;}
    void setLightable(bool nlight) { m_Lightable = nlight;}
    bool isLightable() { return m_Lightable;}
};

class ItemGeneric: public Item
{
private:

public:
    ItemGeneric(std::string nname, chtype nchar, int ncolor) : Item(nname, nchar, ncolor) {};
    int getType() { return I_GENERIC;}
};

class ItemDoor: public Item
{
private:
    chtype m_CharacterDoorOpen;

public:
    ItemDoor(std::string nname, chtype nchar, int ncolor);
    int getType() { return I_DOOR;}

    chtype getCharacterDoorOpen() { return m_CharacterDoorOpen;}
    void setCharacterDoorOpen(chtype ndopen) { m_CharacterDoorOpen = ndopen;}
};

////////////////////////////////////////////////////////////////////////////////
//

class ItemInstance
{
protected:
    Item *m_ItemRef;

    vector2i m_Position;

public:
    ItemInstance(Item *iref, int x, int y);
    virtual int getType() = 0;

    vector2i getPosition() { return m_Position;}
    void setPosition(vector2i newpos) { m_Position = newpos;}
    void setPosition(int x, int y) {m_Position.x = x; m_Position.y = y;}

    std::string getName() { return m_ItemRef->getName();}
    int getColor() {return m_ItemRef->getColor();}
    virtual chtype getCharacter() { return m_ItemRef->getCharacter();}
    virtual bool isWalkable() { return m_ItemRef->isWalkable();}
    virtual bool isLightable() { return m_ItemRef->isLightable();}
};

class ItemInstanceGeneric: public ItemInstance
{
private:

public:
    ItemInstanceGeneric(Item *iref, int x, int y) : ItemInstance(iref, x, y) {};
    int getType() { return m_ItemRef->getType();}
};

class ItemInstanceDoor: public ItemInstance
{
private:
    bool m_DoorOpen;
public:
    ItemInstanceDoor(Item *iref, int x, int y) : ItemInstance(iref, x, y) {};
    int getType() { return m_ItemRef->getType();}

    chtype getCharacter();
    bool isWalkable();
    bool isLightable();

    bool isDoorOpen() { return m_DoorOpen;}
    void toggleDoor() { m_DoorOpen = !m_DoorOpen;}
    void setDoorOpen(bool ndoor) { m_DoorOpen = ndoor;}
};
#endif // CLASS_ITEM
