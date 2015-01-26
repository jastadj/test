#ifndef COMMON_HEADER
#define COMMON_HEADER

struct vector2i
{
    int x;
    int y;
};

struct recti
{
    int x;
    int y;
    int width;
    int height;
};

float getSlantInt(int x1, int y1, int x2, int y2);
float getDistanceInt(int x1, int y1, int x2, int y2);

#endif // COMMON_HEADER
