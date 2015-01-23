#include "engine.hpp"
#include "colors.hpp"

Engine::Engine()
{

}

void Engine::start()
{
    initCurses();
    initColors();
    initTiles();

    //set map window dimensions
    m_MapWindow.x = 0;
    m_MapWindow.y = 0;
    m_MapWindow.width = 40;
    m_MapWindow.height = 25;

    newGame();

    mainLoop();
}

void Engine::initCurses()
{
    initscr();

    //allow extended keyboard
    keypad(stdscr, TRUE);

    //allow scrolling
    scroll(stdscr);

    //halfdelay - non blocking getch
    //halfdelay(1);

}

void Engine::initColors()
{
    start_color();

    init_pair(DEFAULT_COLOR, COLOR_WHITE, COLOR_BLACK);
    init_pair(RED, COLOR_RED, COLOR_BLACK);
    init_pair(GREEN, COLOR_GREEN, COLOR_BLACK);
    init_pair(BLUE, COLOR_BLUE, COLOR_BLACK);
}

void Engine::initTiles()
{
    MapTile newtile;

    //tile 0 : empty
    m_Tiles.push_back(newtile);

    //tile 1 : wall
    newtile = MapTile();
    newtile.m_Character = 219;
    newtile.m_Walkable = false;
    m_Tiles.push_back(newtile);

    //tile 2 : floor
    newtile = MapTile();
    newtile.m_Character = '.';
    m_Tiles.push_back(newtile);


}
////////////////////////////////////////////////////////////////////////////
//
void Engine::newGame()
{
    //randomize seed
    srand( time(NULL));

    initMap();
    initPlayer();
}

void Engine::initMap()
{
    //clear current map
    m_currentMap = NULL;

    //clear map list
    m_Maps.clear();

    //create first map
    Map newmap(MAP_WIDTH, MAP_HEIGHT);
    newmap.genDungeon();
    m_Maps.push_back(newmap);

    //set current map as first map
    m_currentMap = &m_Maps[0];
}

void Engine::initPlayer()
{
    if(m_Player != NULL) delete m_Player;

    m_Player = new Player();
    m_Player->setPosition(5,5);
}
/////////////////////////////////////////////////////////////////////////////
//
void Engine::mainLoop()
{
    bool quit = false;

    int ch = 0;

    //configure curses
    noecho();
    curs_set(0);

    while(!quit)
    {
        //clear screen
        clear();

        //calc each frame
        vector2i mapTopLeft = m_Player->getPosition();
        mapTopLeft.x -= m_MapWindow.width/2;
        mapTopLeft.y -= m_MapWindow.height/2;



        //draw
        drawMap(mapTopLeft, m_MapWindow);
        drawPlayer(m_MapWindow.x + m_MapWindow.width/2, m_MapWindow.y + m_MapWindow.height/2);

        //debug
        mvprintw(24,0,"key:%d", ch);

        ch = getch();

        //handle keyboard input
        if(ch == 27) quit = true;
        //move player SW
        else if(ch == 49)
        {
            m_Player->setPosition( moveDirection(m_Player->getPosition(), D_SOUTHWEST));
        }
        //move player SOUTH
        else if(ch == 258 || ch == 50)
        {
            m_Player->setPosition( moveDirection(m_Player->getPosition(), D_SOUTH));
        }
        //move player SE
        else if(ch == 51)
        {
            m_Player->setPosition( moveDirection(m_Player->getPosition(), D_SOUTHEAST));
        }
        //move player WEST
        else if(ch == 260 || ch == 52)
        {
            m_Player->setPosition( moveDirection(m_Player->getPosition(), D_WEST));
        }
        //move player EAST
        else if(ch == 261 || ch == 54)
        {
            m_Player->setPosition( moveDirection(m_Player->getPosition(), D_EAST));
        }
        //move player NW
        else if(ch == 261 || ch == 55)
        {
            m_Player->setPosition( moveDirection(m_Player->getPosition(), D_NORTHWEST));
        }
        //move player NORTH
        else if(ch == 259 || ch == 56)
        {
            m_Player->setPosition( moveDirection(m_Player->getPosition(), D_NORTH));
        }
        //move player NE
        else if(ch == 261 || ch == 57)
        {
            m_Player->setPosition( moveDirection(m_Player->getPosition(), D_NORTHEAST));
        }

    }
}

/////////////////////////////////////////////////////////
//
void Engine::drawPlayer(int x, int y)
{

    mvaddch(y, x, '@');
}

void Engine::drawMap(vector2i maptopleftpos, recti mapwindow)
{
    int mapwidth = m_currentMap->getWidth();
    int mapheight = m_currentMap->getHeight();

    for(int i = maptopleftpos.y; i < mapwindow.height + maptopleftpos.y; i++)
    {
        for(int n = maptopleftpos.x; n < mapwindow.width + maptopleftpos.x; n++)
        {
            if(i < 0 || n < 0 || i >= mapheight || n >= mapwidth) continue;

            //set tile color
            attron(COLOR_PAIR( m_Tiles[m_currentMap->getMapTile(n,i)].m_Color));
            //draw tile character at position
            mvaddch(i - maptopleftpos.y, n - maptopleftpos.x , m_Tiles[m_currentMap->getMapTile(n, i)].m_Character);
            //set tile color back to default color (white)
            attron(COLOR_PAIR(1));
        }
    }
}

bool Engine::inFov(int x, int y)
{
    //some sight radius calculations should be done first to determine
    //if this check should be performed in the first place...
    need to clean this up

    int px = m_Player->getPosition().x;
    int py = m_Player->getPosition().y;

    int x1 = x;
    int y1 = y;

    //if checking tile player is on
    if(px-x1 == 0 && py-y1 == 0) return true;

    //if los lands on y axis
    if(px-x1 == 0)
    {
        //start from target and walk back to plr pos
        for(int i = y1; i != py; i+= (py-y1)/abs(py-y1))
        {
            //ignore first tile
            if(i == y1) continue;

            //if i is north of pgy
            if(i < 0)
            {
                //if target tile is passable return true
                if( !mMapTiles[mActiveMaps[1 +(gy-pgy)][1 + (gx-pgx)]->getTile(x,MAP_HEIGHT+i)].mPassable) return false;
            }
            //if i is south of
            else if( i >= MAP_HEIGHT)
            {
                if( !mMapTiles[mActiveMaps[1 +(gy-pgy)][1 + (gx-pgx)]->getTile(x,i - MAP_HEIGHT)].mPassable) return false;
            }
            else //if tile is within p map
            {
                if( !mMapTiles[mActiveMaps[1][1]->getTile(x,i)].mPassable) return false;
            }
        }

        //check passed, return true
        return true;

    }
    else //if tile lands on x axis
    if(py-y1 == 0)
    {
       //start from target and walk back to plr pos
        for(int i = x1; i != px; i+= (px-x1)/abs(px-x1))
        {
            //ignore first tile
            if(i == x1) continue;

            //if i is north of pgx
            if(i < 0)
            {
                //if target tile is passable return true
                if( !mMapTiles[mActiveMaps[1 +(gy-pgy)][1 + (gx-pgx)]->getTile(MAP_WIDTH+i,y)].mPassable) return false;
            }
            //if i is south of pgx
            else if( i >= MAP_WIDTH)
            {
                if( !mMapTiles[mActiveMaps[1 +(gy-pgy)][1 + (gx-pgx)]->getTile(i - MAP_WIDTH,y)].mPassable) return false;
            }
            else //if tile is within p map
            {
                if( !mMapTiles[mActiveMaps[1][1]->getTile(i,y)].mPassable) return false;
            }
        }

        //check passed, return true
        return true;
    }

    //y = mx+b
    float m = (float(y1-py)/float(x1-px) );
    float b = py - (m*px);

    //if coords are on the same active map as plr
    //if(gx == pgx && gy == pgy)
    //{
        for(int i = x1; i != px; i+= (px-x1)/abs(px-x1))
        {
            //if first tile, ignore
            if(i == x1) continue;

            int tempx = i;
            int tempy = round(m*i+b);
            int tempgx = pgx;
            int tempgy = pgy;

            //make adjustments as necessary to reflect local coordinate
            if(tempy < 0) {tempy = tempy + MAP_HEIGHT; tempgy--;}
            if(tempy >= MAP_HEIGHT) {tempy = tempy - MAP_HEIGHT; tempgy++;}
            if(tempx < 0) {tempx = tempx + MAP_WIDTH; tempgx--;}
            if(tempx >= MAP_WIDTH) {tempx = tempx - MAP_WIDTH; tempgx++;}

            //check to see if target tile is passable
            if( !mMapTiles[mActiveMaps[1 + (tempgy-pgy)][1 + (tempgx-pgx)]->getTile(tempx, tempy)].mPassable) return false;
        }

        for(int i = y1; i != py; i+= (py-y1)/abs(py-y1))
        {
            //if first tile, ignore
            if(i == y1) continue;

            int tempx = round((i-b)/m);
            int tempy = i;
            int tempgx = pgx;
            int tempgy = pgy;

            //make adjustments as necessary to reflect local coordinates
            if(tempy < 0) {tempy = tempy + MAP_HEIGHT; tempgy--;}
            if(tempy >= MAP_HEIGHT) {tempy = tempy - MAP_HEIGHT; tempgy++;}
            if(tempx < 0) {tempx = tempx + MAP_WIDTH; tempgx--;}
            if(tempx >= MAP_WIDTH) {tempx = tempx - MAP_WIDTH; tempgx++;}

            //check to see if target tile is passable
            if( !mMapTiles[mActiveMaps[1 + (tempgy-pgy)][1 + (tempgx-pgx)]->getTile( tempx, tempy)].mPassable) return false;
        }

        return true;
    //}



    return false;
}

///////////////////////////////////////////////////////////////
//
vector2i Engine::moveDirection(vector2i startpos, int dir)
{
    vector2i newpos = startpos;

    switch(dir)
    {
    case D_SOUTHWEST:
        if(startpos.y < m_currentMap->getHeight()-1 &&
           startpos.x > 0)
        {newpos.y++; newpos.x--;}
        break;
    case D_SOUTH:
        if(startpos.y < m_currentMap->getHeight()-1) newpos.y++;
        break;
    case D_SOUTHEAST:
        if(startpos.y < m_currentMap->getHeight()-1 &&
           startpos.x < m_currentMap->getWidth()-1)
        {newpos.y++; newpos.x++;}
        break;
    case D_WEST:
        if(startpos.x > 0) newpos.x--;
        break;
    case D_EAST:
        if(startpos.x < m_currentMap->getWidth()-1) newpos.x++;
        break;
    case D_NORTHWEST:
        if(startpos.y > 0&&
           startpos.x > 0)
        {newpos.y--; newpos.x--;}
        break;
    case D_NORTH:
        if(startpos.y > 0) newpos.y--;
        break;
    case D_NORTHEAST:
        if(startpos.y > 0 &&
           startpos.x < m_currentMap->getWidth()-1)
        {newpos.y--; newpos.x++;}
        break;
    default:
        break;
    }

    //check tile for walkable
    if(!m_Tiles[m_currentMap->getMapTile(newpos.x, newpos.y)].m_Walkable) return startpos;

    return newpos;
}
