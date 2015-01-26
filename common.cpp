#include "common.hpp"
#include <cmath>

float getSlantInt(int x1, int y1, int x2, int y2)
{
    if(x1 == x2 && y1 == y2) return 0;

    int rise = y2 - y1;
    int run = x2 - x1;

    if(rise == 0) return run;
    else if(run == 0) return rise;
    else return float(rise)/float(run);
}

float getDistanceInt(int x1, int y1, int x2, int y2)
{
    float rise = y2 - y1;
    float run = x2 - x1;

    return sqrtf(rise*rise + run*run);
}
