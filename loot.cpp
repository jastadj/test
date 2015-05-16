#include "loot.hpp"

#include <iostream>
#include <cstdlib>

LootGenerator::LootGenerator()
{
    //default loot capacity
    lootCapacity = 1;
}

void LootGenerator::addLoot(Item *additem, int findchance)
{
    if(additem == NULL)
    {
        std::cout << "Error adding item to loot list - item is null!\n";
        return;
    }

    randomLoot rloot;
    rloot.item = additem;
    rloot.findchance = findchance;

    lootList.push_back(rloot);
}

std::vector<Item*> LootGenerator::generateLoot(int seed)
{
    std::vector<Item*> newloot;

    //set random seed
    srand(seed);

    //walk through loot list and roll, if 100% chance, automatically add
    for(int i = 0; i < int(lootList.size()); i++)
    {
        if(lootList[i].findchance >= 100) newloot.push_back(lootList[i].item);
        else
        {
            if(rand()%100 < lootList[i].findchance ) newloot.push_back(lootList[i].item);
        }
    }

    return newloot;
}
