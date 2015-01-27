#include "engine.hpp"
#include "colors.hpp"
#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
#include <time.h>

Engine::Engine()
{
    //initialize pointers
    m_Player = NULL;
}

void Engine::start()
{
    initCurses();
    std::cout << "init curses\n";
    initColors();
    std::cout << "init colors\n";
    initTiles();
    std::cout << "init tiles\n";
    initItems();
    std::cout << "init items\n";

    //set map window dimensions
    m_MapWindow.x = 0;
    m_MapWindow.y = 0;
    m_MapWindow.width = 40;
    m_MapWindow.height = 25;

    //debug
    d_light = false;

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
    init_pair(YELLOW, COLOR_YELLOW, COLOR_BLACK);
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
    newtile.m_Lightable = false;
    m_Tiles.push_back(newtile);

    //tile 2 : floor
    newtile = MapTile();
    newtile.m_Character = '.';
    m_Tiles.push_back(newtile);


}

void Engine::initItems()
{
    Item newitem("rock", '*', 1);
    m_Items.push_back(newitem);

    newitem = Item("food", '%', YELLOW);
    m_Items.push_back(newitem);

}
////////////////////////////////////////////////////////////////////////////
//
void Engine::newGame(int nseed)
{
    //randomize seed
    if(nseed == -1) m_Seed = int( time(NULL));
    else m_Seed = nseed;
    srand(m_Seed);

    initMap();
    std::cout << "init map\n";
    initPlayer();
    std::cout << "init player \n";
}

void Engine::initMap()
{
    //clear current map
    m_currentMap = NULL;

    //clear map list
    for(int i = int(m_Maps.size()-1); i >= 0; i--)
    {
        delete m_Maps[i];
    }
    m_Maps.clear();

    //create first map
    Map *newmap = new Map(MAP_WIDTH, MAP_HEIGHT);
    newmap->fillMap(2);
    newmap->genDungeon();
    m_Maps.push_back(newmap);

    //set current map as first map
    m_currentMap = m_Maps[0];

    //test
    //add some random items to current map
    //add some random items
    clear();
    for(int i = 0; i < 5; i++)
    {
        int x = rand()%m_currentMap->getWidth();
        int y = rand()%m_currentMap->getHeight();

        if( m_Tiles[m_currentMap->getMapTile(x,y)].m_Walkable)
        {
            ItemInstance *newitem = new ItemInstance(&m_Items[rand()%2], x, y);
            m_currentMap->addItem(newitem);
        }
    }
}

void Engine::initPlayer()
{
    if(m_Player != NULL) delete m_Player;

    m_Player = new Player();
    m_Player->setPosition(5,5);
    m_Player->setVisRadius(12);
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
        else if(ch == 269)
        {
            d_debugmenu();
        }
        else if(ch == 103)
        {
            PlayerGetItem();
        }
        else if(ch == 100)
        {
            PlayerDropItem();
        }
        else if(ch ==  105)
        {
            PlayerInventory();
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

    int playerx = m_Player->getPosition().x;
    int playery = m_Player->getPosition().y;

    for(int i = maptopleftpos.y; i < mapwindow.height + maptopleftpos.y; i++)
    {
        for(int n = maptopleftpos.x; n < mapwindow.width + maptopleftpos.x; n++)
        {
            if(i < 0 || n < 0 || i >= mapheight || n >= mapwidth) continue;

            //check to see if tile is in visual range
            //this is broken
            if( std::abs(getDistanceInt(playerx, playery, n, i)) > m_Player->getVisRadius() && !d_light) continue;

            //check if tile is visible to player
            if(!inFov(playerx, playery, n, i) && !d_light ) continue;

            //if there is an item there, draw that instead
            std::vector<ItemInstance*> founditems = m_currentMap->getItemsAt(n,i);
            //if items are found at tile, draw last item instead of tile
            if(!founditems.empty())
            {
                //set tile color
                attron(COLOR_PAIR( founditems.back()->getColor() ) );
                //draw tile character at position
                mvaddch(i - maptopleftpos.y, n - maptopleftpos.x , founditems.back()->getCharacter() );
                //set tile color back to default color (white)
                attron(COLOR_PAIR(1));
            }
            //otherwise draw tile
            else
            {
                //set tile color
                attron(COLOR_PAIR( m_Tiles[m_currentMap->getMapTile(n,i)].m_Color));
                //draw tile character at position
                mvaddch(i - maptopleftpos.y, n - maptopleftpos.x , m_Tiles[m_currentMap->getMapTile(n, i)].m_Character);
                //set tile color back to default color (white)
                attron(COLOR_PAIR(1));
            }
        }
    }
}

bool Engine::inFov(int sx, int sy, int tx, int ty)
{
    //sx and sy = source tile
    //tx and ty = target tile

    //if same tile
    if(sx == tx && sy == ty) return true;

    //if on y-axis relative to source
    else if(sx == tx)
    {
        //if above source
        if(ty < sy)
        {
            for(int i = sy-1; i > ty; i--)
            {
                if( !m_Tiles[m_currentMap->getMapTile(sx, i)].m_Lightable) return false;
            }
            return true;
        }
        else if(ty > sy)
        {
            for(int i = sy+1; i < ty; i++)
            {
                if( !m_Tiles[m_currentMap->getMapTile(sx, i)].m_Lightable) return false;
            }
            return true;
        }
    }
    //if on x-axis relative to source
    else if(sy == ty)
    {
        //if to the left of source
        if(tx < sx)
        {
            for(int i = sx-1; i > tx; i--)
            {
                if( !m_Tiles[m_currentMap->getMapTile(i, sy)].m_Lightable) return false;
            }
            return true;
        }
        //if to the right of source
        else if(tx > sx)
        {
            for(int i = sx+1; i < tx; i++ )
            {
                if( !m_Tiles[m_currentMap->getMapTile(i, sy)].m_Lightable) return false;
            }
            return true;
        }
    }

    //check around all quadrants
    //y = mx+b
    float m = (float(ty-sy)/float(tx-sx) );
    float b = sy - (m*sx);

    //sweep for x-axis accuracy
    for(int i = tx; i != sx; i+= (sx-tx)/abs(sx-tx))
    {
        //if first tile, ignore
        if(i == tx) continue;

        int tempx = i;
        int tempy = round(m*i+b);

        //check to see if target tile is passable
        if( !m_Tiles[m_currentMap->getMapTile(tempx, tempy)].m_Lightable) return false;
    }

    for(int i = ty; i != sy; i+= (sy-ty)/abs(sy-ty))
    {
        //if first tile, ignore
        if(i == ty) continue;

        int tempx = round((i-b)/m);
        int tempy = i;

        //check to see if target tile is passable
        if( !m_Tiles[m_currentMap->getMapTile(tempx, tempy)].m_Lightable) return false;
    }

    return true;
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

void Engine::PlayerGetItem()
{
    vector2i playerpos = m_Player->getPosition();

    //get list of items at players position
    std::vector<ItemInstance*> itemspresent = m_currentMap->getItemsAt(playerpos.x, playerpos.y);

    if(itemspresent.empty()) return;

    //get last item from items at players current position
    ItemInstance *titem = itemspresent.back();

    //remove item from map
    m_currentMap->removeItem(titem);
    //add item to player
    m_Player->addItem(titem);

}

void Engine::PlayerDropItem()
{
    clear();

    std::vector<ItemInstance*> *playeritems = m_Player->getInventory();

    int ch = 0;

    for(int i = 0; i < int(playeritems->size()); i++)
    {
        printw("%d - %s\n", i, (*playeritems)[i]->getName().c_str());
    }

    ch = getch();

    ch = ch-48;

    //if selection is out of bounds
    if(ch < 0 || ch >= int(playeritems->size()) ) return;
    //else drop item
    else
    {
        ItemInstance *titem = (*playeritems)[ch];

        //remove item from player
        m_Player->removeItem(titem);
        //add item to map at players position
        titem->setPosition(m_Player->getPosition());
        m_currentMap->addItem(titem);

    }

}

void Engine::PlayerInventory()
{
    clear();

    //get players inventory
    std::vector<ItemInstance*> *playeritems = m_Player->getInventory();

    printw("You are carrying:\n\n");

    ///if player is carrying nothing
    if(playeritems->empty()) printw("Nothing!\n");
    //else print inventory
    else
    {
        for(int i = 0; i < int(playeritems->size()); i++)
        {
            printw("%s\n", (*playeritems)[i]->getName().c_str());
        }
    }
    getch();
}
///////////////////////////////////////////////////////////////
//
void Engine::d_debugmenu()
{

    bool quit = false;

    int selection = 0;

    while(!quit)
    {
        clear();

        int ch = 0;

        std::vector<std::string> menuitem;

        menuitem.push_back("Show Map Items");
        if(!d_light)menuitem.push_back("FOV Debug = Off");
        else menuitem.push_back("FOV Debug = On");

        printw("DEBUG\n");
        printw("-----\n\n");

        mvprintw(0,20,"Player Pos:%d,%d", m_Player->getPosition().x, m_Player->getPosition().y);
        mvprintw(1,20,"Seed:%d", m_Seed);

        for(int i = 0; i  < int(menuitem.size()); i++)
        {
            if(selection == i) attron(A_REVERSE);
            mvprintw(i+4 + i*2, 4,"%s\n\n", menuitem[i].c_str());
            attroff(A_REVERSE);
        }

        ch = getch();

        if(ch == 27) quit = true;
        //down pressed
        else if(ch ==  258)
        {
            selection++;
            if(selection >= int(menuitem.size()) ) selection = 0;
        }
        //up
        else if(ch == 259)
        {
            selection--;
            if(selection < 0) selection = int(menuitem.size()-1);
        }
        //enter
        else if(ch == 10)
        {
            if(selection == 0) d_showmapitems();
            else if(selection == 1) d_light = !d_light;
        }
    }
}

void Engine::d_showmapitems()
{
    clear();

    std::vector<ItemInstance*> *mapitems = m_currentMap->d_getmapitems();

    for(int i = 0; i < int(mapitems->size()); i++)
    {
        printw("%s\n", (*mapitems)[i]->getName().c_str());
    }
    getch();
}
