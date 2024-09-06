#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;

class Actor : public GraphObject
{
public:
	Actor(int id, int startx, int starty, int dir, int depth, StudentWorld* ptr) :GraphObject(id, startx, starty, dir, depth, 1.0) { studentwrld = ptr; alive = true; m_impacted = false; }
	virtual void doSomething() = 0;
	StudentWorld* GetWorld() { return studentwrld; }
	bool PeachisOn(); //check if players are on the tile; doesnt account for stopped or not
	bool YoshiisOn();
	virtual bool isEnemy() { return false; }
	bool isAlive() { return alive; }
	void SetAlive(bool value) { alive = value; }
	virtual bool impacted() { return m_impacted; }
	void WasImpacted() { m_impacted = true; }
	void SetImpactedFalse() { m_impacted = false; }
	int RandomLegalMove();
	bool isOnFork(int walkdir);
	bool isOnSquare();
private:
	StudentWorld* studentwrld;
	bool alive;
	bool m_impacted;
};

class Square : public Actor  //all squares are nonimpactable
{
public:
	Square(int id, int startx, int starty, int dir, StudentWorld* ptr) :Actor(id, startx, starty, dir, 1, ptr) { impactable = false; }
private:
	bool impactable;
};

class Vortex : public Actor
{
public:
	Vortex(int startx, int starty, int dir, StudentWorld* ptr) :Actor(IID_VORTEX, startx, starty, dir, 0, ptr) { m_dir = dir; }
	void doSomething();
private:
	int m_dir;
};

class CoinSquare : public Square
{
public:
	CoinSquare(int id, int startx, int starty, StudentWorld* ptr) :Square(id, startx, starty,right, ptr) { }
	void doSomething();
private:
};

class StarSquare : public Square
{
public:
	StarSquare(int startx, int starty, StudentWorld* ptr): Square(IID_STAR_SQUARE,startx, starty,right, ptr){}
	void doSomething();
};

class DirectionalSquare : public Square
{
public:
	DirectionalSquare(int startx, int starty, int dir, StudentWorld* ptr) : Square(IID_DIR_SQUARE, startx, starty, dir, ptr) { forcingdir = dir; }
	void doSomething();
private:
	int forcingdir;
};

class BankSquare : public Square
{
public:
	BankSquare(int startx, int starty, StudentWorld* ptr) : Square(IID_BANK_SQUARE, startx, starty, right, ptr) {}
	void doSomething();
};

class EventSquare : public Square
{
public:
	EventSquare(int startx, int starty, StudentWorld* ptr) : Square(IID_EVENT_SQUARE, startx, starty, right, ptr) {}
	void doSomething();
	void Swap();
};

class DroppingSquare : public Square
{
public:
	DroppingSquare(int startx, int starty, StudentWorld* ptr) : Square(IID_DROPPING_SQUARE, startx, starty, right, ptr) {}
	void doSomething();
};

class Enemy : public Actor
{
public:
	Enemy(int id, int startx, int starty, int dir, StudentWorld* ptr) :Actor(id, startx, starty, dir, 0, ptr) 
	{
		initialtravel = 0; walking = false; pausecounter = 180; squares_to_move = 0; ticks_to_move = 0; walkdir = right; 
	}
	bool isWalking() { return walking; }
	void isWalking(bool value) { walking = value; }
	int PauseCounter() { return pausecounter; }
	void DecrementPauseCounter() { pausecounter--; }
	void SetPauseCounter(int number) { pausecounter = number; }
	void SquaresToMove(int number) { squares_to_move = number; }
	void SetTicksToMove(int ticks) { ticks_to_move = ticks; }
	int TicksToMove() { return ticks_to_move; }
	void DecrementTicksToMove() { ticks_to_move--; }
	bool isOnTurningPoint();
	int WalkDir() { return walkdir; }
	void SetWalkDir(int dir) { walkdir = dir; }
	void WalkRules(int maxmoves);
	bool isEnemy() { return true; }
	void enemyImpacted();


private:
	int initialtravel;
	bool walking;
	int pausecounter;
	int squares_to_move;
	int ticks_to_move;
	int walkdir;
	

};

class Bowser : public Enemy
{
public:
	Bowser(int startx, int starty, StudentWorld* ptr) :Enemy(IID_BOWSER, startx, starty, right, ptr) {}
	void doSomething();
};

class Boo : public Enemy
{
public:
	Boo(int startx, int starty, StudentWorld* ptr) :Enemy(IID_BOO, startx, starty, right, ptr) {}
	void doSomething();
};

class Player: public Actor    //pointer to studentworld?
{
public:
	Player(int id, int playerid, int startx, int starty, StudentWorld* ptr);
	void doSomething();
	bool isWalking() { return m_walking; }
	void SetWalking(bool walk) { m_walking = walk; }
	int coins() { return m_coins; }
	void modifyCoins(int modifier);
	void SetCoins(int coins) { m_coins = coins; }
	int stars() { return m_stars; }
	void SetStars(int stars) { m_stars = stars; }
	int dieRoll() { return die_roll; }
	void addStar() { m_stars++; }
	void minusStar();
	int WalkDir() { return walkdir; }
	void updateWalkDir(int dir) { walkdir = dir; }
	bool newplayer() { return m_newplayer; }
	void newplayer(bool value) { m_newplayer = value; }
	bool encountered() { return m_encountered; }
	void encountered(bool value) { m_encountered = value; }
	void move();
	void SetSwapped() { swapped = true; }
	bool giveVortex() { return vortex = true; }
	bool hasVortex() { return vortex; }
	int tickstomove() { return ticks_to_move; }
	void SetTicks(int ticks) { ticks_to_move = ticks; }
	void teleport();


private:
	bool m_walking;
	int m_playerid;
	int m_coins; 
	int m_stars;
	int ticks_to_move;
	int die_roll;
	int walkdir;
	bool vortex;
	bool m_newplayer; // has player moved or not, default set to false (no coins at first tile), then set to true when walking
	// true when walking, and can only be false if object gives it somethign, and then has to wait until its movign again
	bool m_encountered;  //diff from newplayer because newplayer will specifically be for tiles, and this for enemies on top of tiles
	bool swapped;
	bool teleported;
};



#endif // ACTOR_H_
