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

ItemDoor::ItemDoor(std::string nname, chtype nchar, int ncolor):Item(nname, nchar, ncolor)
{
    m_CharacterDoorOpen = '\\';
}

///////////////////////////////////////////////////
//

ItemInstance::ItemInstance(Item *iref, int x, int y)
{
    //link item instance to reference item (master)
    m_ItemRef = iref;
    //set item instances position
    m_Position.x = x;
    m_Position.y = y;
}

chtype ItemInstanceDoor::getCharacter()
{

    if(m_DoorOpen)
    {
        ItemDoor *doorptr = dynamic_cast<ItemDoor*>(m_ItemRef);
        return doorptr->getCharacterDoorOpen();
    }
    else return m_ItemRef->getCharacter();
}

bool ItemInstanceDoor::isWalkable()
{
    if(m_DoorOpen) return true;

    else return m_ItemRef->isWalkable();
}

bool ItemInstanceDoor::isLightable()
{
    if(m_DoorOpen) return true;

    else return m_ItemRef->isLightable();
}
