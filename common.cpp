#include "common.hpp"
#include <cmath>

float getSlantInt(int x1, int y1, int x2, int y2)
{
    if(x1 == x2 && y1 == y2) return 0;

    float rise = std::abs(y2-y1);
    float run = std::abs(x2-x1);

    if(rise == 0) return run;
    else if(run == 0) return rise;
    else return rise/run;
}
