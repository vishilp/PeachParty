#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Board.h"
#include <string>
#include "Actor.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Player; 
class Actor;

class StudentWorld : public GameWorld
{
public:
  StudentWorld(std::string assetPath);
  ~StudentWorld();
  virtual int init();
  virtual int move();
  virtual void cleanUp();
  bool isValidMove(int col, int row,Player* player);
  Player* getPeach() { return players[0]; }
  Player* getYoshi() { return players[1]; }
  Board::GridEntry TileType(int x, int y) { return bd.getContentsOf(x, y); }
  std::string GameText();
  void BankCoinModifier(int coins);
  int BankCoins() { return m_bankmoney; }
  void InsertDroppingSquare(int x, int y);
  void Destroy();
  bool VortexOverlap(int vortexx, int vortexy);
  void CreateVortex(int x, int y, int dir);
  int isOnDirectional(int x, int y); // -1 if no, or output dir


private:
	int m_bankmoney;
	Board bd;
	Player* players[2];
	std::vector<Actor*> actors;
};

#endif // STUDENTWORLD_H_
