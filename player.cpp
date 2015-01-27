#include "player.hpp"

Player::Player()
{
    m_Pos.x = 0;
    m_Pos.y = 0;

    m_visRadius = 2;
}

Player::~Player()
{
    for(int i = int(m_Inventory.size()-1); i >= 0; i--)
    {
        delete m_Inventory[i];
    }

    m_Inventory.clear();
}

void Player::addItem(ItemInstance *nitem)
{
    m_Inventory.push_back(nitem);
}

void Player::removeItem(ItemInstance *nitem)
{
    for(int i = int(m_Inventory.size()-1); i >= 0; i--)
    {
        if(m_Inventory[i] == nitem)
        {
            m_Inventory.erase(m_Inventory.begin() + i);
            return;
        }
    }
}
