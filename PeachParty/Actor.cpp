#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

bool Actor::PeachisOn()
{
	StudentWorld* ptr = GetWorld();
	Player* peach = ptr->getPeach();
	int x = peach->getX();
	int y = peach->getY();
	if (getX() == x && getY() == y)
		return true;
	return false;
}

bool Actor::YoshiisOn()
{
	StudentWorld* ptr = GetWorld();
	Player* yoshi = ptr->getYoshi();
	int x = yoshi->getX();
	int y = yoshi->getY();
	if (getX() == x && getY() == y)
		return true;
	return false;
}


void CoinSquare::doSomething()
{
	if (!isAlive())
		return;
	
	Board::GridEntry square = GetWorld()->TileType(getX() / SPRITE_WIDTH, getY() / SPRITE_HEIGHT); // could be reversed x and y
	//check if coins are being distributed by couting
	if (PeachisOn() && GetWorld()->getPeach()->newplayer() == true && GetWorld()->getPeach()->isWalking() == false)
	{
		if (square == Board::blue_coin_square || square == Board::player || square == Board::bowser || square == Board::boo)
		{
			GetWorld()->getPeach()->modifyCoins(3);
			GetWorld()->playSound(SOUND_GIVE_COIN);
		}
		else
		{
			GetWorld()->getPeach()->modifyCoins(-3);
			GetWorld()->playSound(SOUND_TAKE_COIN);
		}
		GetWorld()->getPeach()->newplayer(false);
	}
	
	if (YoshiisOn()&& GetWorld()->getYoshi()->newplayer() == true && GetWorld()->getYoshi()->isWalking()== false)
	{
		if (square == Board::blue_coin_square || square == Board::player)
			GetWorld()->getYoshi()->modifyCoins(3);
		else
			GetWorld()->getYoshi()->modifyCoins(-3);
		GetWorld()->getYoshi()->newplayer(false);
	}
	
	return; 
}

void StarSquare::doSomething()
{
	StudentWorld* ptr = GetWorld();
	Player* peach = ptr->getPeach();
	if (peach->newplayer() == true)
	{
		if (PeachisOn()==true)
		{
			if (peach->coins() >= 20)
			{
				peach->modifyCoins(-20);
				peach->addStar();
				ptr->playSound(SOUND_GIVE_STAR);
				peach->newplayer(false);
			}
		}
	}

	Player* yoshi = ptr->getYoshi();
	if (yoshi->newplayer() == true)
	{
		if (YoshiisOn() == true)
		{
			if (yoshi->coins() >= 20)
			{
				yoshi->modifyCoins(-20);
				yoshi->addStar();
				ptr->playSound(SOUND_GIVE_STAR);
				yoshi->newplayer(false);
			}
		}
	}
	
	return;
}

void DirectionalSquare::doSomething()
{
	if (PeachisOn() == true)
		GetWorld()->getPeach()->updateWalkDir(forcingdir);


	if (YoshiisOn() == true)
		GetWorld()->getYoshi()->updateWalkDir(forcingdir);

	return;
}

void BankSquare::doSomething()
{
	if (PeachisOn() == true && GetWorld()->getPeach()->isWalking() == false)
	{
		if (GetWorld()->getPeach()->newplayer() == true)
		{
			int coins = GetWorld()->BankCoins();
			GetWorld()->getPeach()->modifyCoins(coins);
			GetWorld()->BankCoinModifier(-coins);
			GetWorld()->getPeach()->newplayer(false);
			GetWorld()->playSound(SOUND_WITHDRAW_BANK);
		}
	}

	if (YoshiisOn() == true && GetWorld()->getYoshi()->isWalking() == false)
	{
		if (GetWorld()->getYoshi()->newplayer() == true)
		{
			int coins = GetWorld()->BankCoins();
			GetWorld()->getYoshi()->modifyCoins(coins);
			GetWorld()->BankCoinModifier(-coins);
			GetWorld()->getYoshi()->newplayer(false);
			GetWorld()->playSound(SOUND_WITHDRAW_BANK);
		}
	}

	if (PeachisOn()== true && GetWorld()->getPeach()->isWalking() == true)
	{
			int coins = GetWorld()->getPeach()->coins();
			if (coins >= 5)
			{
				GetWorld()->BankCoinModifier(5);
				GetWorld()->getPeach()->modifyCoins(-5);
			}
			else
			{
				GetWorld()->BankCoinModifier(coins);
				GetWorld()->getPeach()->modifyCoins(-coins);
			}
			GetWorld()->playSound(SOUND_DEPOSIT_BANK);
	}

	if (YoshiisOn()== true && GetWorld()->getYoshi()->isWalking() == true)
	{
		int coins = GetWorld()->getYoshi()->coins();
		if (coins >= 5)
		{
			GetWorld()->BankCoinModifier(5);
			GetWorld()->getYoshi()->modifyCoins(-5);
		}
		else
		{
			GetWorld()->BankCoinModifier(coins);
			GetWorld()->getYoshi()->modifyCoins(-coins);
		}
		GetWorld()->playSound(SOUND_DEPOSIT_BANK);
	}
	return;
}

void EventSquare::Swap()
{
	int Px = GetWorld()->getPeach()->getX();
	int Py = GetWorld()->getPeach()->getY();
	int Pticks = GetWorld()->getPeach()->tickstomove();
	int Pdir = GetWorld()->getPeach()->WalkDir();
	int Pspritedir = GetWorld()->getPeach()->getDirection();
	bool Pwalking = GetWorld()->getPeach()->isWalking();

	int Yx = GetWorld()->getYoshi()->getX();
	int Yy = GetWorld()->getYoshi()->getY();
	int Yticks = GetWorld()->getYoshi()->tickstomove();
	int Ydir = GetWorld()->getYoshi()->WalkDir();
	int Yspritedir = GetWorld()->getYoshi()->getDirection();
	bool Ywalking = GetWorld()->getYoshi()->isWalking();

	GetWorld()->getPeach()->moveTo(Yx, Yy);
	GetWorld()->getPeach()->SetTicks(Yticks);
	GetWorld()->getPeach()->updateWalkDir(Ydir);
	GetWorld()->getPeach()->setDirection(Yspritedir);
	GetWorld()->getPeach()->SetWalking(Ywalking);

	GetWorld()->getYoshi()->moveTo(Px, Py);
	GetWorld()->getYoshi()->SetTicks(Pticks);
	GetWorld()->getYoshi()->updateWalkDir(Pdir);
	GetWorld()->getYoshi()->setDirection(Pspritedir);
	GetWorld()->getYoshi()->SetWalking(Pwalking);
}

void EventSquare::doSomething()
{
	if (PeachisOn() == true && GetWorld()->getPeach()->isWalking() == false && GetWorld()->getPeach()->newplayer() == true)
	{
		int option = randInt(1,3);
		switch (option)
		{
		case 1:
			GetWorld()->getPeach()->teleport();
			GetWorld()->playSound(SOUND_PLAYER_TELEPORT);
			break;
		case 2:
		{
			Swap();
			GetWorld()->getPeach()->newplayer(false);
			GetWorld()->getPeach()->SetSwapped();
			GetWorld()->playSound(SOUND_PLAYER_TELEPORT);
			break;
		}
		case 3:
			GetWorld()->getPeach()->giveVortex();
			GetWorld()->playSound(SOUND_GIVE_VORTEX);
			break;
		}
		GetWorld()->getPeach()->newplayer(false);
	}
	if (YoshiisOn() == true && GetWorld()->getYoshi()->isWalking() == false && GetWorld()->getYoshi()->newplayer() == true)
	{
		int option = randInt(1,3);
		switch (option)
		{
		case 1:
			GetWorld()->getYoshi()->teleport();
			GetWorld()->playSound(SOUND_PLAYER_TELEPORT);
			break;
		case 2:
		{
			Swap();
			GetWorld()->getYoshi()->newplayer(false);
			GetWorld()->getYoshi()->SetSwapped();
			GetWorld()->playSound(SOUND_PLAYER_TELEPORT);
			break;
		}
		case 3:
			GetWorld()->getYoshi()->giveVortex();
			GetWorld()->playSound(SOUND_GIVE_VORTEX);
			break;
		}
		GetWorld()->getYoshi()->newplayer(false);
	}
	return;
}


void DroppingSquare::doSomething()
{
	if (PeachisOn() == true && GetWorld()->getPeach()->isWalking() == false)
	{
		if (GetWorld()->getPeach()->newplayer() == true)
		{
			int option = randInt(1, 2);
			if (option == 1)
			{
				GetWorld()->getPeach()->modifyCoins(-10);
			}
			else
			{
				GetWorld()->getPeach()->minusStar();
			}
			GetWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
			GetWorld()->getPeach()->newplayer(false);
		}
	}
	
	if (YoshiisOn() == true && GetWorld()->getYoshi()->isWalking() == false)
	{
		if (GetWorld()->getYoshi()->newplayer() == true)
		{
			int option = randInt(1, 2);
			if (option == 1)
			{
				GetWorld()->getYoshi()->modifyCoins(-10);
			}
			else
			{
				GetWorld()->getYoshi()->minusStar();
			}
			GetWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
			GetWorld()->getYoshi()->newplayer(false);
		}
	}
	return;
}

int Actor::RandomLegalMove()
{
	int x = getX() / 16;
	int y = getY() / 16;
	Board::GridEntry square;
	int arr[] = { -1,-1,-1,-1, -1};
	square = GetWorld()->TileType(x+1, y); //right
	if (square != Board::empty)
		arr[0] = 0;

	square = GetWorld()->TileType(x, y+1); //up
	if (square != Board::empty)
		arr[1] = 90;

	square = GetWorld()->TileType(x-1, y); //left
	if (square != Board::empty)
		arr[2] = 180;

	square = GetWorld()->TileType(x, y - 1); //down
	if (square != Board::empty)
		arr[3] = 270;

	int j = 4;
	while (arr[j] == -1)
	{
		j = randInt(0, 3);
	}

	return arr[j];


}

bool Actor::isOnSquare()
{
	int x = getX();
	int y = getY();
	if (x % 16 == 0 && y % 16 == 0)
		return true;
	return false;
}

bool Actor::isOnFork(int walkdir) //if there are two or more directions, but not left/right or up/down
{
	Board::GridEntry sprite;

	switch (walkdir)
	{
	case right:
	case left:
			if (GetWorld()->TileType(getX() / 16, getY() / 16 + 1) != Board::empty && GetWorld()->TileType(getX() / 16, getY() / 16 - 1) != Board::empty)
				return true;
			if (GetWorld()->TileType(getX() / 16 + 1, getY() / 16) != Board::empty && GetWorld()->TileType(getX() / 16 - 1, getY() / 16) != Board::empty && GetWorld()->TileType(getX() / 16, getY() / 16 + 1))
				return true;
			if (GetWorld()->TileType(getX() / 16 + 1, getY() / 16) != Board::empty && GetWorld()->TileType(getX() / 16 - 1, getY() / 16) != Board::empty && GetWorld()->TileType(getX() / 16, getY() / 16 - 1))
				return true;
			break;
	case up:
	case down:
			if (GetWorld()->TileType(getX() / 16+1, getY() / 16) != Board::empty&& GetWorld()->TileType(getX() / 16 - 1, getY() / 16) != Board::empty)
				return true;
			if (GetWorld()->TileType(getX() / 16, getY() / 16 - 1) != Board::empty && GetWorld()->TileType(getX() / 16, getY() / 16 + 1) != Board::empty && GetWorld()->TileType(getX() / 16 + 1, getY() / 16) != Board::empty)
				return true;
			if (GetWorld()->TileType(getX() / 16, getY() / 16 - 1) != Board::empty && GetWorld()->TileType(getX() / 16, getY() / 16 + 1) != Board::empty && GetWorld()->TileType(getX() / 16 - 1, getY() / 16) != Board::empty)
				return true;
			break;
	}

	
	return false;
}

bool Enemy::isOnTurningPoint()
{
	Board::GridEntry sprite;
	switch (walkdir)
	{
	case (right):
		sprite = GetWorld()->TileType(getX() / 16 + 1, getY() / 16);
		if (sprite == Board::empty)
			return true;
		break;
	case (up):
		sprite = GetWorld()->TileType(getX() / 16, getY() / 16+1);
		if (sprite == Board::empty)
			return true;
		break;
	case (left):
		sprite = GetWorld()->TileType(getX() / 16-1, getY() / 16);
		if (sprite == Board::empty)
			return true;
		break;
	case (down):
		sprite = GetWorld()->TileType(getX() / 16, getY() / 16 - 1);
		if (sprite == Board::empty)
			return true;
		break;
	}
	return false;
}

void Bowser::doSomething()
{
	if (isWalking() == false)  //1a
	{
		if (PeachisOn() == true && GetWorld()->getPeach()->isWalking() == false)
		{
			if (GetWorld()->getPeach()->encountered() == false)
			{
				int option = randInt(1, 2);
				if (option == 1)
				{
					int coins = GetWorld()->getPeach()->coins();
					int stars = GetWorld()->getPeach()->stars();
					GetWorld()->getPeach()->modifyCoins(-coins);
					for (; stars > 0; stars--)
					{
						GetWorld()->getPeach()->minusStar();
					}
					GetWorld()->playSound(SOUND_BOWSER_ACTIVATE);
				}
				GetWorld()->getPeach()->encountered(true);
			}
		}

		if (YoshiisOn() == true && GetWorld()->getYoshi()->isWalking() == false)
		{
			if (GetWorld()->getYoshi()->encountered() == false)
			{
				int option = randInt(1, 2);
				if (option == 1)
				{
					int coins = GetWorld()->getYoshi()->coins();
					int stars = GetWorld()->getYoshi()->stars();
					GetWorld()->getYoshi()->modifyCoins(-coins);
					for (; stars > 0; stars--)
					{
						GetWorld()->getYoshi()->minusStar();
					}
					GetWorld()->playSound(SOUND_BOWSER_ACTIVATE);
				}
				GetWorld()->getYoshi()->encountered(true);
			}
		}
	}

	WalkRules(10);
	if (isWalking())
	{
		DecrementTicksToMove();
		if (TicksToMove() == 0)
		{
			isWalking(false);
			SetPauseCounter(180);
			int option = randInt(1, 4);
			if (option == 1)
				GetWorld()->InsertDroppingSquare(getX() / 16, getY() / 16); //make sure characters dont get deleted
		}
	}

	if (impacted())
		enemyImpacted();
	return;
}

void Enemy::enemyImpacted()
{
	Board::GridEntry sprite = Board::empty;
	int x, y;
	while (sprite == Board::empty)
	{
		x = randInt(1, 16);
		y = randInt(1, 16);
		sprite = GetWorld()->TileType(x, y);
	}
	moveTo(x * 16, y * 16);
	SetWalkDir(right);
	setDirection(0);
	SetPauseCounter(180);
	isWalking(false);
	SetImpactedFalse();
}

void Enemy::WalkRules(int maxmoves)
{
	if (isWalking() == false)  //1b & c
	{
		DecrementPauseCounter();
		int counter = PauseCounter();
		if (counter == 0)
		{
			int move = randInt(1, maxmoves);
			SquaresToMove(move);
			SetTicksToMove(move * 8);
			int dir = RandomLegalMove();
			if (dir == 0)
			{
				setDirection(0);
				SetWalkDir(right);
			}
			if (dir == 90)
			{
				setDirection(0);
				SetWalkDir(up);
			}
			if (dir == 180)
			{
				setDirection(180);
				SetWalkDir(left);
			}
			if (dir == 270)
			{
				setDirection(0);
				SetWalkDir(down);
			}
			isWalking(true);
		}
	}

	if (isWalking())
	{
		if (isOnSquare() && isOnFork(WalkDir()))
		{
			int dir = RandomLegalMove();
			if (dir == 180)
				setDirection(180);
			else
				setDirection(0);
			SetWalkDir(dir);
		}
		else if (isOnSquare() && isOnTurningPoint())
		{
			switch (WalkDir())
			{
			case left:
			case right:
				setDirection(0);
				SetWalkDir(up);
				if (GetWorld()->TileType(getX() / 16, getY() / 16 + 1) == Board::empty)
					SetWalkDir(down);
				break;
			case down:
			case up:

				setDirection(0);
				SetWalkDir(right);
				if (GetWorld()->TileType(getX() / 16 + 1, getY() / 16) == Board::empty)
				{
					setDirection(180);
					SetWalkDir(left);
				}
				break;
			}

		}
	}

	if (isWalking())
	{
		switch (WalkDir())
		{
		case right:
			moveTo(getX() + 2, getY());
			break;

		case left:
			moveTo(getX() - 2, getY());
			break;
		case up:
			moveTo(getX(), getY() + 2);
			break;
		case down:
			moveTo(getX(), getY() - 2);
			break;
		default:
			break;
		}

	}
}
void Boo::doSomething()
{
	if (isWalking() == false)
	{
		if (PeachisOn() == true && GetWorld()->getPeach()->isWalking() == false)
		{
			if (GetWorld()->getPeach()->encountered() == false)
			{
				int option = randInt(1, 2);
				if (option == 1)
				{
					int Pcoins = GetWorld()->getPeach()->coins();
					int Ycoins = GetWorld()->getYoshi()->coins();
					GetWorld()->getPeach()->SetCoins(Ycoins);
					GetWorld()->getYoshi()->SetCoins(Pcoins);
				}
				if (option == 2)
				{
					int Pstars = GetWorld()->getPeach()->stars();
					int Ystars = GetWorld()->getYoshi()->stars();
					GetWorld()->getPeach()->SetStars(Ystars);
					GetWorld()->getYoshi()->SetStars(Pstars);
				}
				GetWorld()->playSound(SOUND_BOO_ACTIVATE);
				GetWorld()->getPeach()->encountered(true);
			}
		}
		if (YoshiisOn() == true && GetWorld()->getYoshi()->isWalking() == false)
		{
			if (GetWorld()->getYoshi()->encountered() == false)
			{
				int option = randInt(1, 2);
				if (option == 1)
				{
					int Pcoins = GetWorld()->getPeach()->coins();
					int Ycoins = GetWorld()->getYoshi()->coins();
					GetWorld()->getPeach()->SetCoins(Ycoins);
					GetWorld()->getYoshi()->SetCoins(Pcoins);
				}
				if (option == 2)
				{
					int Pstars = GetWorld()->getPeach()->stars();
					int Ystars = GetWorld()->getYoshi()->stars();
					GetWorld()->getPeach()->SetStars(Ystars);
					GetWorld()->getYoshi()->SetStars(Pstars);
				}
				GetWorld()->playSound(SOUND_BOO_ACTIVATE);
				GetWorld()->getYoshi()->encountered(true);
			}
		}
		
	}

	WalkRules(3);
	DecrementTicksToMove();
	if (TicksToMove() == 0)
	{
		isWalking(false);
		SetPauseCounter(180);
	}

	if (impacted())
		enemyImpacted();
}


void Vortex::doSomething()
{
	if (!isAlive())
		return;

	switch (m_dir)
	{
	case right:
		moveTo(getX() + 2, getY());
		break;

	case left:
		moveTo(getX() - 2, getY());
		break;
	case up:
		moveTo(getX(), getY() + 2);
		break;
	case down:
		moveTo(getX(), getY() - 2);
		break;
	}

	if (getX() < 0 || getX() >= VIEW_WIDTH || getY() < 0 || getY() >= VIEW_HEIGHT)
	{
		SetAlive(false);
		GetWorld()->Destroy();
		return;
	}

	bool hit = GetWorld()->VortexOverlap(getX(), getY());
	if (hit == true)
	{
		SetAlive(false);
		GetWorld()->playSound(SOUND_HIT_BY_VORTEX);
		GetWorld()->Destroy();
	}

	return;
}

Player::Player(int id, int playerid,  int startx, int starty, StudentWorld* ptr)
	:Actor(id, SPRITE_WIDTH* startx, SPRITE_HEIGHT* starty, right, 0, ptr)
{
	m_coins = 0;
	m_stars = 0;
	m_walking = false;
	m_playerid = playerid; 
	ticks_to_move = 0; 
	die_roll = 0; 
	walkdir = right;
	vortex = false;
	m_newplayer = false;
	m_encountered = false;
	teleported = false;
	swapped = false;
}

void Player::move()
{
	switch (walkdir)
	{
	case right:
		moveTo(getX() + 2, getY());
		break;

	case left:
		moveTo(getX() - 2, getY());
		break;
	case up:
		moveTo(getX(), getY() + 2);
		break;
	case down:
		moveTo(getX(), getY() - 2);
		break;
	default:
		break;
	}

	ticks_to_move--;
	if (ticks_to_move == 0)
	{
		m_walking = false;
	}
	return;
}

void Player::teleport()
{
	Board::GridEntry sprite = Board::empty;
	int x, y;
	while (sprite == Board::empty)
	{
		x = randInt(1, 16);
		y = randInt(1, 16);
		sprite = GetWorld()->TileType(x, y);
	}
	moveTo(x*16, y*16);
	teleported = true;
	m_walking = false;
	
}

void Player::doSomething()
{
	if (m_walking == false)
	{
		die_roll = 0;
		if (teleported == true) //pick ur own dir if random tp
		{
			int action = GetWorld()->getAction(m_playerid);
			switch (action)
			{
			case ACTION_ROLL:
				return;
			case ACTION_LEFT:
			{
					walkdir = left;
					bool result = GetWorld()->isValidMove(getX(), getY(), this);
					if (result == true)
						break;
					else
						return;
			}
			case ACTION_RIGHT:
			{
					walkdir = right;
					bool result = GetWorld()->isValidMove(getX(), getY(), this);
					if (result == true)
						break;
					else
						return;
			}
			case ACTION_UP:
			{
					walkdir = up;
					bool result = GetWorld()->isValidMove(getX(), getY(), this);
					if (result == true)
						break;
					else
						return;
			}
			case ACTION_DOWN:
			{
					walkdir = down;
					bool result = GetWorld()->isValidMove(getX(), getY(), this);
					if (result == true)
						break;
					else
						return;
			}
			case ACTION_FIRE:
				return;
			case ACTION_NONE:
				return;
			}
			teleported = false;
		}

		else if (swapped == true && GetWorld()->isValidMove(getX(), getY(), this) == false)
		{
			int dir = RandomLegalMove();
			if (dir == left)
			{
				setDirection(left);
				walkdir = left;
			}
			else if (dir == right)
			{
				setDirection(right);
				walkdir = right;
			}
			else if (dir == up)
			{
				setDirection(right);
				walkdir = up;
			}
			else
			{
				setDirection(down);
				walkdir = down;
			}
		}
		int move = GetWorld()->getAction(m_playerid);
		switch (move)
		{
		case ACTION_NONE:
			return;
		case ACTION_LEFT:
			return;
		case ACTION_RIGHT:
			return;
		case ACTION_UP:
			return;
		case ACTION_DOWN:
			return;
		case ACTION_ROLL:
			die_roll = randInt(1, 10);
			ticks_to_move = die_roll * 8;
			m_walking = true;
			break;
		case ACTION_FIRE:
			if (vortex)
			{
				switch (walkdir)
				{
				case left:
					GetWorld()->CreateVortex(getX() -16, getY(), walkdir);
					break;
				case right:
					GetWorld()->CreateVortex(getX() +16, getY(), walkdir);
					break;
				case up:
					GetWorld()->CreateVortex(getX(), getY()+16, walkdir);
					break;
				case down:
					GetWorld()->CreateVortex(getX(), getY()-16, walkdir);
					break;
				}
				vortex = false;
			}
			break;
		}
	}
	if (m_walking)
	{
		m_newplayer = true;
		m_encountered = false;
		teleported = false;
		swapped = false;
	}


	if (m_walking)  //directional check
	{
		int directional = GetWorld()->isOnDirectional(getX(), getY());
		if (directional != -1)
		{
			switch (directional)
			{
			case right:
				setDirection(right);
				walkdir = right;
				break;
			case left:
				setDirection(left);
				walkdir = left;
				break;
			case up:
				setDirection(right);
				walkdir = up;
				break;
			case down:
				setDirection(right);
				walkdir = down;
				break;
			}

			move();
			return;
		}

	}

	if (m_walking) //fork check
	{
		if (isOnSquare() && isOnFork(walkdir)) // has to roll first
		{
			int action = GetWorld()->getAction(m_playerid);
			int originaldir = walkdir;
			switch (action)
			{
			case ACTION_ROLL:
				return;
			case ACTION_LEFT:
			{
				if (originaldir == right) // cant go backwards
					return;
				else
				{
					walkdir = left;
					bool result =GetWorld()->isValidMove(getX(), getY(), this);
					if (result == false)
					{
						walkdir = originaldir;
						return;
					}
					else
						setDirection(left);
				}
				break;
			}
			case ACTION_RIGHT:
			{
				if (originaldir == left) // cant go backwards
					return;
				else
				{
					walkdir = right;
					bool result = GetWorld()->isValidMove(getX(), getY(), this);
					if (result == false)
					{
						walkdir = originaldir;
						return;
					}
					else
						setDirection(right);
				}
				break;
			}
			case ACTION_UP:
			{
				if (originaldir == down) // cant go backwards
					return;
				else
				{
					walkdir = up;
					bool result = GetWorld()->isValidMove(getX(), getY(), this);
					if (result == false)
					{
						walkdir = originaldir;
						return;
					}
					else
						setDirection(right);
				}
				break;
			}
			case ACTION_DOWN:
			{
				if (originaldir == up) // cant go backwards
					return;
				else
				{
					walkdir = down;
					bool result = GetWorld()->isValidMove(getX(), getY(), this);
					if (result == false)
					{
						walkdir = originaldir;
						return;
					}
					else
						setDirection(right);
				}
				break;
			}
			case ACTION_FIRE:
				return;
			case ACTION_NONE:
				return;
			}
			
			
			move();
			return;
		}
	}
	if (m_walking)
	{
		if (ticks_to_move % 8 == 0)  //make sure the tile and character are aligned
		{
			die_roll--;
			switch (walkdir)
			{
			case right:
				if (GetWorld()->isValidMove(getX(), getY(), this) == false) //going right
				{
					setDirection(0);
					walkdir = up;
					if (GetWorld()->isValidMove(getX(), getY(), this) == false)
						walkdir = down;
				}
				break;
			case left:
				if (GetWorld()->isValidMove(getX(), getY(), this) == false) //going left
				{
					setDirection(0);
					walkdir = up;
					if (GetWorld()->isValidMove(getX(), getY(), this) == false)
						walkdir = down;
				}
				break;
			case up:

				if (GetWorld()->isValidMove(getX(), getY(), this) == false) //going up
				{
					setDirection(0);
					walkdir = right;
					if (GetWorld()->isValidMove(getX(), getY(), this) == false)
					{
						setDirection(180);
						walkdir = left;
					}
				}
				break;
			case down:

				if (GetWorld()->isValidMove(getX(), getY(), this) == false) //going down
				{
					setDirection(0);
					walkdir = right;
					if (GetWorld()->isValidMove(getX() + 2, getY(), this) == false)
					{
						setDirection(180);
						walkdir = left;
					}
				}
				break;
			}
		}


		move();
		return;
	}

}

	

void Player::modifyCoins(int modifier)
{
	m_coins += modifier;
	if (m_coins < 0)
		m_coins = 0;
}

void Player::minusStar()
{
	m_stars--;
	if (m_stars < 0)
		m_stars = 0;
}

