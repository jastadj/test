#include "astar.hpp"

AStar::AStar(std::vector< std::vector<int> > *tmap, int startx, int starty, int endx, int endy)
{
    m_Map = tmap;

    m_StartX = startx;
    m_StartY = starty;
    m_EndX = endx;
    m_EndY = endy;

}

AStar::~AStar()
{

}

void AStar::getHeuristic(A_Node *tnode)
{
    int ydelta = std::abs(m_EndY - tnode->y);
    int xdelta = std::abs(m_EndX - tnode->x);

    tnode->h = (ydelta + xdelta)*A_ORTHOCOST;
}

void AStar::getFscore(A_Node *tnode)
{
    tnode->f = tnode->h + tnode->g;
}

int AStar::validTile(int x, int y)
{
    // return 0 if outside map bounds
    //  return 0 if in closed list
    // return 0 if unwalkable
    // return 1 if in openllist
    // return 2 if totally valid

    //in map bounds?
    if(x < 0 || x >= int((*m_Map)[0].size()) || y < 0 || y >= int(m_Map->size()) ) return 0;

    //walkable?
    else if( (*m_Map)[y][x] == 1) return 0;

    //in openlist?
    else if( inList( &m_OpenList, x, y)) return 1;

    //in closed list?
    else if( inList( &m_ClosedList, x, y)) return 0;


    return 2;
}
bool AStar::inList( std::vector<A_Node*> *tlist, int x, int y)
{
    for(int i = 0; i < int(tlist->size()); i++)
    {
        if( (*tlist)[i]->x == x && (*tlist)[i]->y == y) return true;
    }

    return false;
}

void AStar::moveNodeToClosedList( A_Node *tnode)
{
    //find node in open list
    for(int i = int(m_OpenList.size() -1); i >= 0; i--)
    {
        if(m_OpenList[i] == tnode)
        {
            //add tnode to closed list
            m_ClosedList.push_back(tnode);

            //remove tnode from open list
            m_OpenList.erase(m_OpenList.begin() + i);
            return;
        }
    }
}

A_Node *AStar::getOpenNode(int x, int y)
{
    for(int i = 0; i < int(m_OpenList.size()); i++)
    {
        if(m_OpenList[i]->x == x && m_OpenList[i]->y == y) return m_OpenList[i];
    }

    return NULL;
}

A_Node *AStar::getLowestFscoreNode()
{
    A_Node *fnode = NULL;
    //if openlist is empty?
    if(m_OpenList.empty()) return fnode;

    //init fnode as first node in open list
    fnode = m_OpenList[0];

    //walk through open list and find highest f score
    for(int i = 0; i < int(m_OpenList.size()); i++)
    {
        if(fnode->f > m_OpenList[i]->f) fnode = m_OpenList[i];
    }

    return fnode;
}

std::vector<vector2i> AStar::findPath()
{
    std::vector<vector2i> pathlist;
    A_Node *currentNode = NULL;
    bool found = false;

    //create starting node
    A_Node *node = new A_Node;
    node->x = m_StartX;
    node->y = m_StartY;
    node->parent = NULL;
    node->g = 0;
    getHeuristic(node);
    getFscore(node);


    //add starting node to open list
    m_OpenList.push_back(node);
    currentNode = m_OpenList[0];

    //process starting node
    //check valid adjacent tiles to add to open list
    for(int n = -1 + currentNode->y; n < 2 + currentNode->y; n++)
    {
        for(int p = -1 + currentNode->x; p < 2 + currentNode->x; p++)
        {
            //dont check center node since it is current node position!
            if(n == 0 && p == 0) continue;

            //check if walkable tile
            if( validTile(p, n) )
            {
                //add it to open list
                A_Node *nnode = new A_Node;
                nnode->x = p;
                nnode->y = n;
                nnode->parent = currentNode;
                getHeuristic(nnode);
                //get g score
                if( (p == -1 || p == 1 ) && (n == -1 || p == 1))
                {
                    nnode->g = currentNode->g + A_DIAGCOST;
                }
                else nnode->g = currentNode->g + A_ORTHOCOST;

                //calc f score
                getFscore(nnode);

                //add to open list
                m_OpenList.push_back(nnode);

            }

        }
    } // end adding valid adjacent tiles to open list

    //drop starting node from open list and add to closed list
    moveNodeToClosedList(currentNode);

    //process openlist
    //for now, just use an iterative run
    while(!m_OpenList.empty() || !found)
    {
        //set current node as lowest F score node
        currentNode = getLowestFscoreNode();

        if(currentNode == NULL) break;

        //check valid adjacent tiles to add to open list
        for(int n = -1 + currentNode->y; n < 2 + currentNode->y; n++)
        {
            //if path was found, break out
            if(found) break;

            for(int p = -1 + currentNode->x; p < 2 + currentNode->x; p++)
            {
                //dont check center node since it is current node position!
                if(n == 0 && p == 0) continue;

                //check if current tile is target tile
                if(p == m_EndX && n == m_EndY)
                {

                    A_Node *lastnode = currentNode;

                    //add up path x/y coords to list to send out
                    //walk backwards up to starting tile
                    while(lastnode->parent != NULL )
                    {
                        vector2i vpath;
                        vpath.x = lastnode->x;
                        vpath.y = lastnode->y;
                        pathlist.push_back( vpath);

                        lastnode = lastnode->parent;
                    }
                    found = true;
                    break;

                }
                //check surrounding tiles
                else
                {
                    int valid = validTile(p,n);

                    //check if walkable tile
                    if( valid )
                    {


                        //add it to open list
                        A_Node *nnode = new A_Node;
                        nnode->x = p;
                        nnode->y = n;
                        nnode->parent = currentNode;
                        getHeuristic(nnode);
                        //get g score
                        if( (p == -1 || p == 1 ) && (n == -1 || p == 1))
                        {
                            nnode->g = currentNode->g + A_DIAGCOST;
                        }
                        else nnode->g = currentNode->g + A_ORTHOCOST;

                        //if tile already in open list, check to see who has
                        //a better g score (lower = better)
                        if(valid == 1)
                        {
                            A_Node *oldNode = getOpenNode(p, n);

                            //if new path is better than old path (less is better)
                            if(nnode->g < oldNode->g)
                            {
                                //set old nodes parent to this one's parent (better path)
                                oldNode->parent = nnode->parent;
                            }

                            //delete new nnode
                            delete nnode;
                        }
                        //otherwise add new node
                        else
                        {

                            //calc f score
                            getFscore(nnode);

                            //add to open list
                            m_OpenList.push_back(nnode);
                        }

                    }
                }

            }
        } // end adding valid adjacent tiles to open list

        //move current node to closed list
        moveNodeToClosedList(currentNode);

    }


    return pathlist;
}

