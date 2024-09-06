#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include "Actor.h"  
#include <vector>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    m_bankmoney = 0;
    players[0] = nullptr;
    players[1] = nullptr;
}

int StudentWorld::init()
{
    int boardnumber= getBoardNumber();
    std::string boardfile = assetPath() + "board0" + to_string(boardnumber) + ".txt";
    Board::LoadResult result = bd.loadBoard(boardfile);
    if (result!=Board::load_success)
        return GWSTATUS_BOARD_ERROR;
	startCountdownTimer(99);  
    
    int x, y;
    Board::GridEntry sprite; 
    for (x = 0; x < BOARD_WIDTH; x++)   //new object all of these 
    {
        for (y = 0; y < BOARD_HEIGHT; y++)
        {
            sprite = bd.getContentsOf(y, x);
            switch (sprite)
            {
            case Board::empty: 
                 break;
            case Board::player: 
            {
                players[0] = new Player(IID_PEACH, 1, y, x, this); //needs p2
                players[1] = new Player(IID_YOSHI, 2, y, x, this);
                actors.push_back(new CoinSquare(IID_BLUE_COIN_SQUARE ,SPRITE_HEIGHT * y, SPRITE_WIDTH * x, this));
                break;
            }
            case Board::blue_coin_square: 
            {
                actors.push_back(new CoinSquare(IID_BLUE_COIN_SQUARE, SPRITE_HEIGHT * y, SPRITE_WIDTH * x, this));
                break;
            }
            case Board::red_coin_square: 
                actors.push_back(new CoinSquare(IID_RED_COIN_SQUARE, SPRITE_HEIGHT * y, SPRITE_WIDTH * x, this));
                break;
            case Board::left_dir_square: 
                actors.push_back(new DirectionalSquare(SPRITE_HEIGHT * y, SPRITE_WIDTH * x, GraphObject::left, this));
                break;
            case Board::right_dir_square: 
                actors.push_back(new DirectionalSquare(SPRITE_HEIGHT * y, SPRITE_WIDTH * x, GraphObject::right, this));
                break;
            case Board::up_dir_square: 
                actors.push_back(new DirectionalSquare(SPRITE_HEIGHT * y, SPRITE_WIDTH * x, GraphObject::up, this));
                break;
            case Board::down_dir_square: 
                actors.push_back(new DirectionalSquare(SPRITE_HEIGHT * y, SPRITE_WIDTH * x, GraphObject::down, this));
                break;
            case Board::event_square: 
                actors.push_back(new EventSquare(SPRITE_HEIGHT * y, SPRITE_WIDTH * x, this));
                break;
            case Board::bank_square: 
                actors.push_back(new BankSquare(SPRITE_HEIGHT * y, SPRITE_WIDTH * x, this));
                break;
            case Board::star_square: 
            {
                actors.push_back(new StarSquare(SPRITE_HEIGHT * y, SPRITE_WIDTH * x, this));
                break;
            }
            case Board::bowser: 
                actors.push_back(new Bowser(SPRITE_HEIGHT * y, SPRITE_WIDTH * x, this));
                actors.push_back(new CoinSquare(IID_BLUE_COIN_SQUARE, SPRITE_HEIGHT * y, SPRITE_WIDTH * x, this));
                break;
            case Board::boo: 
                actors.push_back(new Boo(SPRITE_HEIGHT * y, SPRITE_WIDTH * x, this));
                actors.push_back(new CoinSquare(IID_BLUE_COIN_SQUARE, SPRITE_HEIGHT * y, SPRITE_WIDTH * x, this));
                break;
            }

        }
    }

    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit ESC.
    // Notice that the return value GWSTATUS_NOT_IMPLEMENTED will cause our framework to end the game.
    
    if (timeRemaining() <= 0)
    {
        playSound(SOUND_GAME_FINISHED);
        int Pstars = players[0]->stars();
        int Ystars = players[1]->stars();
        int Pcoins = players[0]->coins();
        int Ycoins = players[1]->coins();
        if (Pstars > Ystars)
        {
            setFinalScore(Pstars, Pcoins);
            return GWSTATUS_PEACH_WON;
        }
        if (Ystars > Pstars)
        {
            setFinalScore(Ystars, Ycoins);
            return GWSTATUS_YOSHI_WON;
        }
        else
        {
            if (Pcoins >= Ycoins)
            {
                setFinalScore(Pstars, Pcoins);
                return GWSTATUS_PEACH_WON;
            }
            else
            {
                setFinalScore(Ystars, Ycoins);
                return GWSTATUS_YOSHI_WON;
            }
               
        }
    }

    for (int i = 0; i < 2; i++)
    {
        players[i]->doSomething();
    }
    //vector<Actor*>::iterator iter;
    for (int x=0; x< actors.size(); x++)
    {
       actors[x]->doSomething();
    }
    string s = GameText();
    setGameStatText(s);
    
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    vector<Actor*>::iterator iter; 
    for (iter=actors.begin(); iter< actors.end(); iter++)
    {
        delete* iter;
    }
    delete players[0];
    delete players[1];
}

bool StudentWorld::isValidMove(int col, int row, Player* player)
{

    int dir = player->WalkDir();
    Board::GridEntry sprite;
 
        switch (dir)
        {
        case 0:
            sprite = bd.getContentsOf(col / 16+1, row / 16);  
            if (sprite == Board::empty)
                return false;
            else
                return true;
            break;
        case 90:
            sprite = bd.getContentsOf(col / 16, row / 16+1);  //up
            if (sprite == Board::empty)
                return false;
            else
                return true;
            break;
        case 180:
            sprite = bd.getContentsOf(col / 16 -1, row / 16);  //left
            if (sprite == Board::empty)
                return false;
            else
                return true;
            break;
        case 270:
            sprite = bd.getContentsOf(col / 16, row / 16-1);  //down
            if (sprite == Board::empty)
                return false;
            else
                return true;
            break;
        }
        
   
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

string StudentWorld::GameText()
{
    

    ostringstream oss;
    oss << "P1 Roll: " << players[0]->dieRoll() << " Stars: " << players[0]->stars() << " $$: " << players[0]->coins();
    if (players[0]->hasVortex())
        oss << " VOR";
    oss << " | Time: " << timeRemaining() << " | Bank: " << m_bankmoney << " | ";
    oss << "P2 Roll: " << players[1]->dieRoll() << " Stars: " << players[1]->stars() << " $$: " << players[1]->coins();
    if (players[1]->hasVortex())
        oss << " VOR";
    string s = oss.str();
    return s;
}

void StudentWorld::BankCoinModifier(int coins)
{
    m_bankmoney += coins;
    if (m_bankmoney < 0)
        m_bankmoney = 0;
}

void StudentWorld::InsertDroppingSquare(int x, int y)
{
    for (int j = 0; j < actors.size(); j++)
    {
        if (actors[j]->isEnemy()== false) //make sure not enemy
        {
            if (x == actors[j]->getX() / 16 && y == actors[j]->getY() / 16)
            {
                if (bd.getContentsOf(actors[j]->getX() / 16, actors[j]->getY() / 16) == Board::blue_coin_square || bd.getContentsOf(actors[j]->getX() / 16, actors[j]->getY() / 16) == Board::red_coin_square || bd.getContentsOf(actors[j]->getX() / 16, actors[j]->getY() / 16) == Board::player || bd.getContentsOf(actors[j]->getX() / 16, actors[j]->getY() / 16) == Board::bowser || bd.getContentsOf(actors[j]->getX() / 16, actors[j]->getY() / 16) == Board::boo)
                {

                    delete actors[j];
                    actors[j] = new DroppingSquare(x * 16, y * 16, this);
                    playSound(SOUND_DROPPING_SQUARE_CREATED);
                    break;
                }
            }
        }
        
    }
}

void StudentWorld::Destroy()
{
    for (int x = 0; x < actors.size(); x++)
    {
        if (actors[x]->isAlive() == false)
        {
            delete actors[x];
            actors.erase(actors.begin() + x);

        }
    }
    return;
}

bool StudentWorld::VortexOverlap(int vortexX, int vortexY)
{
    int count = 0;
    int lowerX = vortexX - 15;
    int upperX = vortexX + 15;
    int lowerY = vortexY - 15;
    int upperY = vortexY + 15;
    int x = 0;
    for ( x = 0; x < actors.size(); x++)
    {
        if (actors[x]->isEnemy())
        {
            int X = actors[x]->getX();
            int Y = actors[x]->getY();
            if ((X > lowerX && X < upperX) && (Y > lowerY && Y < upperY))
            {
                count++;
                break; //first actor is hit
            }
                
        }
    }
    
    if (count > 0)
    {
        actors[x]->WasImpacted();
        return true;
    }
    return false;
}

void StudentWorld::CreateVortex(int x, int y, int dir)
{
    actors.push_back(new Vortex(x, y,dir, this));
    playSound(SOUND_PLAYER_FIRE);
}

int StudentWorld::isOnDirectional(int x, int y)
{
    if (x % 16 == 0 && y % 16 == 0)
    {
        Board::GridEntry sprite = bd.getContentsOf(x / 16, y / 16);
        if (sprite == Board::left_dir_square)
            return 180;
        if (sprite == Board::right_dir_square)
            return 0;
        if (sprite == Board::up_dir_square)
            return 90;
        if (sprite == Board::down_dir_square)
            return 270;
    }
    return -1;
}