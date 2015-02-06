#ifndef CLASS_ASTAR
#define CLASS_ASTAR

#include <cstdlib>
#include <vector>
#include <cmath>

#include "common.hpp"

#define A_ORTHOCOST 10
#define A_DIAGCOST 14

//http://www.policyalmanac.org/games/aStarTutorial.htm

struct A_Node
{
    A_Node *parent;

    int x;
    int y;

    int g;
    int h;
    int f;
};

class AStar
{
private:

    std::vector< std::vector<int> > *m_Map;

    int m_StartX;
    int m_StartY;

    int m_EndX;
    int m_EndY;

    std::vector<A_Node*> m_OpenList;
    std::vector<A_Node*> m_ClosedList;


    void getHeuristic(A_Node *tnode);
    void getFscore(A_Node *tnode);

    int validTile(int x, int y);
    bool inList( std::vector<A_Node*> *tlist, int x, int y);
    void moveNodeToClosedList( A_Node *tnode);
    A_Node *getOpenNode(int x, int y);
    A_Node *getLowestFscoreNode();


public:
    AStar(std::vector< std::vector<int> > *tmap, int startx, int starty, int endx, int endy);
    ~AStar();

    //this return type needs to change to whatever format is used for posiotions, or movement dirs, etc..
    std::vector<vector2i> findPath();

};

#endif // CLASS_ASTAR
