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
    std::cout << "init curses\n";
    initCurses();
    std::cout << "init colors\n";
    initColors();
    std::cout << "init tiles\n";
    initTiles();
    std::cout << "init items\n";
    initItems();
    std::cout << "init mobs\n";
    initMobs();

    //set map window dimensions
    m_MapWindow.x = 0;
    m_MapWindow.y = 0;
    m_MapWindow.width = 40;
    m_MapWindow.height = 25;

    //debug
    d_light = false;

    std::cout << "start new game\n";
    newGame();
    std::cout << "starting main loop\n";
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
    init_pair(CYAN, COLOR_CYAN, COLOR_BLACK);
    init_pair(MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
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

    //tile 3 : grass
    newtile = MapTile();
    newtile.m_Character = '.';
    newtile.m_Color = GREEN;
    m_Tiles.push_back(newtile);

    //tile 4 : grass
    newtile = MapTile();
    newtile.m_Character = ',';
    newtile.m_Color = GREEN;
    m_Tiles.push_back(newtile);

    //tile 5 : grass
    newtile = MapTile();
    newtile.m_Character = '\'';
    newtile.m_Color = GREEN;
    m_Tiles.push_back(newtile);

    //tile 6 : grass
    newtile = MapTile();
    newtile.m_Character = '`';
    newtile.m_Color = GREEN;
    m_Tiles.push_back(newtile);

    //tile 7 : grass
    newtile = MapTile();
    newtile.m_Character = '"';
    newtile.m_Color = GREEN;
    m_Tiles.push_back(newtile);

    //tile 8 : grass
    newtile = MapTile();
    newtile.m_Character = '.';
    newtile.m_Color = GREEN;
    newtile.m_ColorBright = true;
    m_Tiles.push_back(newtile);

    //tile 9 : grass
    newtile = MapTile();
    newtile.m_Character = ',';
    newtile.m_Color = GREEN;
    newtile.m_ColorBright = true;
    m_Tiles.push_back(newtile);

    //tile 10 : grass
    newtile = MapTile();
    newtile.m_Character = '\'';
    newtile.m_Color = GREEN;
    newtile.m_ColorBright = true;
    m_Tiles.push_back(newtile);

    //tile 11 : grass
    newtile = MapTile();
    newtile.m_Character = '`';
    newtile.m_Color = GREEN;
    newtile.m_ColorBright = true;
    m_Tiles.push_back(newtile);

    //tile 12 : grass
    newtile = MapTile();
    newtile.m_Character = '"';
    newtile.m_Color = GREEN;
    newtile.m_ColorBright = true;
    m_Tiles.push_back(newtile);

    //tile 13 : random grass (used to identify random grass tile for gen)
    newtile = MapTile();
    newtile.m_Character = '?';
    newtile.m_Color = GREEN;
    m_Tiles.push_back(newtile);

}

void Engine::initItems()
{
    Item *newitem = new ItemGeneric("rock", '*', 1);
    m_Items.push_back(newitem);

    newitem = new ItemGeneric("food", '%', YELLOW);
    m_Items.push_back(newitem);

    newitem = new ItemDoor("door", chtype(219), YELLOW);
    newitem->setWalkable(false);
    newitem->setMovable(false);
    newitem->setLightable(false);
    m_Items.push_back(newitem);

}

void Engine::initMobs()
{
    Mob newmob("rat", 'r', 1);
    newmob.setMaxHP(3);
    newmob.setDefaultAIBehavior(AI_WANDER);
    m_Mobs.push_back(newmob);

}
////////////////////////////////////////////////////////////////////////////
//
void Engine::newGame(int nseed)
{
    //randomize seed
    if(nseed == -1) m_Seed = int( time(NULL));
    else m_Seed = nseed;
    srand(m_Seed);

    std::cout << "init map\n";
    initMap();
    std::cout << "init player \n";
    initPlayer();

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
    m_Maps.push_back(newmap);

    //set current map as first map
    m_currentMap = m_Maps[0];

    //test
    //add some random items to current map
    //add some random items
    genMap();

    //add test mob
    MobInstance *newmob = new MobInstance(&m_Mobs[0], 9,9);
    m_currentMap->addMob(newmob);

    for(int i = 0; i < 5; i++)
    {
        int x = rand()%m_currentMap->getWidth();
        int y = rand()%m_currentMap->getHeight();

        if( m_Tiles[m_currentMap->getMapTile(x,y)].m_Walkable)
        {
            ItemInstance *newitem = new ItemInstanceGeneric(m_Items[rand()%2], x, y);
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

    m_Player->setName("test");
    /*
    clear();
    echo();
    char buf[20];
    mvprintw(12,20,"Enter name:");
    getstr(buf);
    noecho();

    m_Player->setName(std::string(buf));
    */


}

/////////////////////////////////////////////////////////////////////////////
//
void Engine::genMap()
{
    //reset seed
    srand(m_Seed);

    //fill map with grass
    mapDrawBox(m_currentMap,0,0, m_currentMap->getWidth(), m_currentMap->getHeight(), 13); // random grass

    mapDrawBox(m_currentMap, 10,10, 15,15, 1);
    mapDrawBox(m_currentMap, 11, 11, 14, 14, 2);
    m_currentMap->setMapTile(10, 13, 2);
    //add door here
    ItemInstance *newitem = new ItemInstanceDoor(m_Items[2], 10,13);
    m_currentMap->addItem(newitem);
    newitem = new ItemInstanceDoor(m_Items[2],3,3);
    m_currentMap->addItem(newitem);

    //test
    m_currentMap->setMapTile(0,0,12);
}

int Engine::genGrass()
{
    //draw random grass tile (3-12)
    return rand()%9+3;
}

void Engine::mapDrawBox(Map *tmap, int x1, int y1, int x2, int y2, int tile)
{

    //order coordinates
    int tx;
    int ty;

    if(x1 > x2)
    {
        tx = x1;
        x1 = x2;
        x2 = tx;
    }

    if(y1 > y2)
    {
        ty = y1;
        y1 = y2;
        y2 = ty;
    }

    for(int i = y1; i <= y2; i++)
    {
        for(int n = x1; n <= x2; n++)
        {
            if( i < 0 || i >= tmap->getHeight() || n < 0 || n >= tmap->getWidth()) continue;

            if(tile == 13) // if tile is random grass
            {
                tmap->setMapTile(n,i,genGrass());
            }
            else tmap->setMapTile(n,i,tile);

        }
    }
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

/*
    WINDOW *mywin = new WINDOW;
    mywin = newwin(5,8,10,10);
*/


    while(!quit)
    {



        //clear screen
        clear();
        //wclear(mywin);

        //mobs turn
        doMobTurn();

        //calc each frame
        vector2i mapTopLeft = m_Player->getPosition();
        mapTopLeft.x -= m_MapWindow.width/2;
        mapTopLeft.y -= m_MapWindow.height/2;

        //draw
        drawMap(mapTopLeft, m_MapWindow);
        drawPlayer(m_MapWindow.x + m_MapWindow.width/2, m_MapWindow.y + m_MapWindow.height/2);
        drawUI(40, 0);
        drawMessages();

        //debug
        mvprintw(24,0,"key:%d", ch);

        refresh();

        //window
        //box(mywin, 0, 0);
        //mvwprintw(mywin,0,1,"TEST");
        //wrefresh(mywin);

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
        else if(ch == 55)
        {
            m_Player->setPosition( moveDirection(m_Player->getPosition(), D_NORTHWEST));
        }
        //move player NORTH
        else if(ch == 259 || ch == 56)
        {
            m_Player->setPosition( moveDirection(m_Player->getPosition(), D_NORTH));
        }
        //move player NE
        else if(ch == 57)
        {
            m_Player->setPosition( moveDirection(m_Player->getPosition(), D_NORTHEAST));
        }
        //f5
        else if(ch == 269)
        {
            d_debugmenu();
        }
        //g or keypad 5
        else if(ch == 103 || ch == 53)
        {
            PlayerGetItem();
        }
        //d
        else if(ch == 100)
        {
            PlayerDropItem();
        }
        //i
        else if(ch ==  105)
        {
            PlayerInventory();
        }
        //c
        else if(ch == 99)
        {
            actionClose();
        }

    }
}

void Engine::addMessage(std::string amsg)
{
    Message newmsg;
    newmsg.msg = amsg;
    newmsg.life = 4;

    m_MessageQue.push_back(newmsg);
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

            //check to see if there is a mob there
            MobInstance *foundmob = m_currentMap->getMobAt(n,i);
            if(foundmob != NULL)
            {

                //set tile color
                attron(COLOR_PAIR( foundmob->getColor() ) );
                //draw tile character at position
                mvaddch(i - maptopleftpos.y, n - maptopleftpos.x , foundmob->getCharacter() );
                //set tile color back to default color (white)
                attron(COLOR_PAIR(1));

            }
            else
            {
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
                    //is tile bright?
                    if(m_Tiles[m_currentMap->getMapTile(n,i)].m_ColorBright) attron(A_BOLD);
                    //draw tile character at position
                    mvaddch(i - maptopleftpos.y, n - maptopleftpos.x , m_Tiles[m_currentMap->getMapTile(n, i)].m_Character);
                    attroff(A_BOLD);
                    //set tile color back to default color (white)
                    attron(COLOR_PAIR(1));
                }
            }
        }
    }
}

void Engine::drawUI(int x, int y)
{
    mvprintw(y+1,x+2, "Name:%s", m_Player->getName().c_str());
    mvprintw(y+2,x+2, "Pos: %d,%d", m_Player->getPosition().x, m_Player->getPosition().y);
    mvprintw(y+3, x+2, "HP: %d/%d", m_Player->getCurrentHP(), m_Player->getMaxHP());
}

void Engine::drawMessages()
{
    int msgcount = int(m_MessageQue.size());

    mvprintw(0,0, "msg count:%d", msgcount);

    for(int i = msgcount-1; i >= 0; i--)
    {
        m_MessageQue[i].life--;

        if(m_MessageQue[i].life < 0) m_MessageQue.erase(m_MessageQue.begin()+i);
        else mvprintw(24 - msgcount + i,2, "%s", m_MessageQue[i].msg.c_str());

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
                if( !posLightable(sx, i) ) return false;
            }
            return true;
        }
        else if(ty > sy)
        {
            for(int i = sy+1; i < ty; i++)
            {
                if( !posLightable(sx, i) ) return false;
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
                if( !posLightable(i, sy) ) return false;
            }
            return true;
        }
        //if to the right of source
        else if(tx > sx)
        {
            for(int i = sx+1; i < tx; i++ )
            {
                if( !posLightable(i, sy) ) return false;
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
        if( !posLightable(tempx, tempy) ) return false;
    }

    for(int i = ty; i != sy; i+= (sy-ty)/abs(sy-ty))
    {
        //if first tile, ignore
        if(i == ty) continue;

        int tempx = round((i-b)/m);
        int tempy = i;

        //check to see if target tile is passable
        if( !posLightable(tempx, tempy) ) return false;
    }

    return true;
}

bool Engine::posLightable(int x, int y)
{
    if( !m_Tiles[m_currentMap->getMapTile(x,y)].m_Lightable) return false;

    //get items at tile and check if they are lightable
    std::vector<ItemInstance*> titems = m_currentMap->getItemsAt(x, y);

    for(int i = 0; i < int(titems.size()); i++)
    {
        if( !titems[i]->isLightable()) return false;
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
    //check if mob occupies tile
    MobInstance *tmob = m_currentMap->getMobAt(newpos.x, newpos.y);
    if( tmob != NULL)
    {
        //attack mob
        tmob->addHP( -m_Player->getAttackDamage());
        //add message
        std::stringstream smsg;
        smsg << "You hit " << tmob->getName() << " for " << m_Player->getAttackDamage() << " dmg";
        addMessage(smsg.str());

        //dont move
        return startpos;
    }
    //check if there is a non-walkable item
    std::vector<ItemInstance*> titems = m_currentMap->getItemsAt(newpos.x, newpos.y);
    if(!titems.empty())
    {
        //check to see if any items are not walkable
        for(int i = 0; i < int(titems.size()); i++)
        {
            if( !titems[i]->isWalkable() )
            {
                //check to see if item is a door?
                if(titems[i]->getType() == I_DOOR)
                {
                    ItemInstanceDoor *dptr = dynamic_cast<ItemInstanceDoor*>(titems[i]);

                    //if door is not open
                    if(!dptr->isDoorOpen())
                    {
                        //open door
                        dptr->setDoorOpen(true);
                        addMessage("You open the door.");
                    }
                }

                return startpos;
            }
        }
    }

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

    if(!titem->isMovable())
    {
        addMessage(std::string("Unable to pickup " + titem->getName()));
        return;
    }

    //remove item from map
    m_currentMap->removeItem(titem);
    //add item to player
    m_Player->addItem(titem);

    //add message
    addMessage(std::string("You pick up " + titem->getName()));

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

        //add message
        addMessage(std::string("You drop " + titem->getName()));

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

void Engine::actionClose()
{
    //get input from user for direction of where to close
    int closedir = 0;

    //query user
    mvprintw(19,5, "Close in which direction?");

    closedir = getch();

    vector2i dpos = m_Player->getPosition();

    switch(closedir)
    {
       //close SW
        case 49:
            m_currentMap->closeDoor(dpos.x-1, dpos.y+1);
            break;
        //close SOUTH
        case 258:
        case 50:
            m_currentMap->closeDoor(dpos.x, dpos.y+1);
            break;
        //close SE
        case 51:
            m_currentMap->closeDoor(dpos.x+1, dpos.y+1);
            break;
        //close WEST
        case 260:
        case 52:
            m_currentMap->closeDoor(dpos.x-1, dpos.y);
            break;
        //close EAST
        case 261:
        case 54:
            m_currentMap->closeDoor(dpos.x+1, dpos.y);
            break;
        //close NW
        case 55:
            m_currentMap->closeDoor(dpos.x-1, dpos.y-1);
            break;
        //close NORTH
        case 259:
        case 56:
            m_currentMap->closeDoor(dpos.x, dpos.y-1);
            break;
        //close NE
        case 57:
            m_currentMap->closeDoor(dpos.x+1, dpos.y-1);
            break;
        default:
            break;
    }

}

void Engine::doMobTurn()
{
    std::vector<MobInstance*> *mobs = m_currentMap->getMobs();

    for(int i = int(mobs->size()-1); i >= 0; i--)
    {
        //if mob is dead
        if( (*mobs)[i]->isDead())
        {
            addMessage(std::string( (*mobs)[i]->getName() + " dies"));
            m_currentMap->removeMob( (*mobs)[i]);
        }

        vector2i mobpos = (*mobs)[i]->getPosition();
        vector2i plypos = m_Player->getPosition();
        getAStarMapChunk(m_currentMap, 30, mobpos.x, mobpos.y, plypos.x, plypos.y);
    }
}

std::vector<std::vector<int> > Engine::getAStarMapChunk(Map *tmap, int radius, int sx, int sy, int tx, int ty)
{
    std::vector< std::vector<int> > pmap;

    //get valid dimensions to clip from map
    recti clip;
    clip.x = sx - radius;
    clip.y = sy - radius;
    clip.width = radius*2;
    clip.height = radius*2;

    //trim rect to fit within map
    //check top
    if(clip.y < 0)
    {
        clip.height += clip.y;
        clip.y = 0;
    }
    //check bottom
    if( (clip.y + clip.height) >= tmap->getHeight())
    {
        int dif = clip.y + clip.height - tmap->getHeight()+1;
        clip.height -= dif;
    }
    //check left
    if(clip.x < 0)
    {
        clip.width += clip.x;
        clip.x = 0;
    }
    //check right
    if( (clip.x + clip.width) >= tmap->getWidth())
    {
        int dif = clip.x + clip.width - tmap->getWidth()+1;
        clip.width -= dif;
    }

    //init vector
    pmap.resize( clip.height);
    for(int i = 0; i < clip.height; i++)
        for(int n = 0; n < clip.width; n++) pmap[i].push_back(0);

    //calculate coordinate offests
    int offset_x = clip.x;
    int offset_y = clip.y;

    //create mask of walkable positions
    //check walkable tiles
    for(int i = clip.y; i < clip.height; i++)
    {
        for(int n = clip.x; n < clip.width; n++)
        {
            if( !m_Tiles[tmap->getMapTile(n,i)].m_Walkable ) pmap[i-offset_y][n-offset_x] = 1;
        }
    }

    //check items
    std::vector<ItemInstance*> *titems = tmap->getItems();
    for(int i = 0; i < int( titems->size()); i++)
    {
        vector2i ipos = (*titems)[i]->getPosition();
        if(!(*titems)[i]->isWalkable()) pmap[ipos.y-offset_y][ipos.x-offset_x] = 1;
    }

    //
    //debug
    /*
    clear();
    printw("map dim: [0-%d], [0-%d]\n", tmap->getWidth()-1, tmap->getHeight()-1);
    printw("source : %d, %d\n", sx, sy);
    printw("target : %d, %d\n\n", tx, ty);
    printw("radius : %d\n", radius);
    printw("clip   : top:%d  bottom:%d    left:%d  right:%d]\n", clip.y, clip.y+clip.height, clip.x, clip.x+clip.width);

    getch();
    */

    return pmap;


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
        printw("%s", (*mapitems)[i]->getName().c_str());
        if( (*mapitems)[i]->isWalkable()) printw(" - walkable");
        printw("\n");
    }
    getch();
}
