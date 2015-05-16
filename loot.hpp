#ifndef CLASS_LOOT
#define CLASS_LOOT

#include "item.hpp"
#include <vector>

struct randomLoot
{
    //item to drop
    Item *item;

    //percentage of chance that this loot will drop (0-100)
    //if chance to find is 100%, then it will always be generated
    //even if the loot count has reached capacity
    short int findchance;

};

class LootGenerator
{
private:

    std::vector<randomLoot> lootList;

    //lootCapacity determines how many items in the list can be
    //generated
    int lootCapacity;

public:
    LootGenerator();

    void addLoot(Item *additem, int findchance);
    void setLootCap(int nlootcap) { lootCapacity = nlootcap;}

    std::vector<Item*> generateLoot(int seed);
};

#endif // CLASS_LOOT
