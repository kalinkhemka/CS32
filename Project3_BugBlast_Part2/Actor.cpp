#include "Actor.h"
#include "StudentWorld.h"
#include <cstdlib>
#include <queue>
using namespace std;

void Player::setDead()
{
	Actor::setDead();
	getWorld()->playSound(SOUND_PLAYER_DIE);
}


void  Player::doSomething()
{
	if (isDead())
		return;
	int x, y;
	getWorld()->getPlayerLocation(x,y);
	Actor* check = getWorld()->objectAtSpot(x,y);
	if (check != nullptr)
		if (check->colocationKillsPlayer())
		{
			setDead();
			return;
		}
	if (m_wallTime> 0)
	{
		m_wallTime--;
		if (m_wallTime == 0)
			m_canWalkThroughWalls = false;
	}
	if (m_sprayerTime > 0)
	{
		m_sprayerTime--;
		if (m_sprayerTime == 0)
		{
			m_maxBugSprayers = 2;
		}
	}
	int ch;
	Actor* actSpot;
	if (getWorld()->getKey(ch))
	{
		switch(ch)
		{
			case KEY_PRESS_LEFT:
				actSpot = getWorld()->objectAtSpot(getX()-1, getY());
				if (actSpot == nullptr && (getX() > 0))
					moveTo(getX()-1, getY());
				else
					if (!actSpot->blocksPlayer() && (getX() > 0))
						moveTo(getX()-1, getY());
				break;
			case KEY_PRESS_RIGHT:
				actSpot = getWorld()->objectAtSpot(getX()+1,getY());
				if (actSpot == nullptr && (getX() < (VIEW_WIDTH-1)))
					moveTo(getX()+1,getY());
				else
					if (!actSpot->blocksPlayer() && (getX() < (VIEW_WIDTH-1)))
						moveTo(getX()+1,getY());
				break;
			case KEY_PRESS_DOWN:
				actSpot = getWorld()->objectAtSpot(getX(),getY()-1);

				if (actSpot == nullptr && (getY() > 0))
					moveTo(getX(),getY()-1);
				else
					if (!actSpot->blocksPlayer() && (getY() > 0))
						moveTo(getX(),getY()-1);
				break;
			case KEY_PRESS_UP:
				actSpot = getWorld()->objectAtSpot(getX(),getY()+1);
				if (actSpot == nullptr && (getY() < (VIEW_HEIGHT-1)))
					moveTo(getX(),getY()+1);
				else
					if (!actSpot->blocksPlayer() && (getY() < (VIEW_HEIGHT-1)))
						moveTo(getX(),getY()+1);
				break;
			case KEY_PRESS_SPACE:
				actSpot = getWorld()->objectAtSpot(getX(),getY());
				if (m_activeBugSprayers >= m_maxBugSprayers)
					break;
				if ((actSpot == nullptr) || (!actSpot->blocksZumiAndSprayers()))
				{
					getWorld()->addActor(new BugSprayer(getWorld(), getX(),getY()));
					m_activeBugSprayers++;
				}
				break;
			default:
				break;
		}
	}

}

bool DestBrick::colocationKillsPlayer() const
{
	if (getWorld()->getPlayer()->canWalkThroughWalls() == true)
		return false;
	else
		return true;
}

bool DestBrick::blocksPlayer() const
{
	if (getWorld()->getPlayer()->canWalkThroughWalls() == true)
		return false;
	else
		return true;
}


void Exit::doSomething()
{
	if (!getWorld()->anyLiveZumi() && !isVisible())
	{
		setVisible(true);
		getWorld()->playSound(SOUND_REVEAL_EXIT);
	}
	if (getWorld()->getCollidingPlayer(this) != nullptr && isVisible())
	{
		getWorld()->playSound(SOUND_FINISHED_LEVEL);
		getWorld()->setLevelFinished();
	}

}

void BugSprayer::doSomething()
{
	if (isDead())
		return;
	if (getLife() > 0)
		decreaseLife();
	if (getLife() == 0)
	{
		int x = getX();
		int y = getY();
		Actor* check;
		getWorld()->playSound(SOUND_SPRAY);
		getWorld()->addActor(new BugSpray(getWorld(), x, y));
		for (int i = 1; i <= 2; i++)
		{
			check = getWorld()->objectAtSpot(x+i,y);
			if (check == nullptr || check->allowsSprayToBeDropped())
				getWorld()->addActor(new BugSpray(getWorld(), x+i, y));
			if (!getWorld()->objectAtSpot(x+i,y)->allowsSprayToPass())
				break;
		}
		for (int i = 1; i <= 2; i++)
		{
			check = getWorld()->objectAtSpot(x-i,y);
			if (check == nullptr || check->allowsSprayToBeDropped())
				getWorld()->addActor(new BugSpray(getWorld(), x-i, y));
			if (!getWorld()->objectAtSpot(x-i,y)->allowsSprayToPass())
				break;
		}
		for (int i = 1; i <= 2; i++)
		{
			check = getWorld()->objectAtSpot(x,y+i);
			if (check == nullptr || check->allowsSprayToBeDropped())
				getWorld()->addActor(new BugSpray(getWorld(), x, y+i));
			if (!getWorld()->objectAtSpot(x,y+i)->allowsSprayToPass())
				break;
		}
		for (int i = 1; i <= 2; i++)
		{
			check = getWorld()->objectAtSpot(x,y-i);
			if (check == nullptr || check->allowsSprayToBeDropped())
				getWorld()->addActor(new BugSpray(getWorld(), x, y-i));
			if (!getWorld()->objectAtSpot(x,y-i)->allowsSprayToPass())
				break;
		}
		setDead();
	}

}
void BugSprayer::setDead()
{
	Actor::setDead();
	getWorld()->getPlayer()->reduceActiveSprayers();
}

void BugSpray::doSomething()
{
	if (isDead())
		return;
	if (getLife() > 0)
		decreaseLife();
	if (getLife() == 0)
		setDead();
	int x = getX();
	int y = getY();
	Actor* check = getWorld()->objectAtSpot(x,y);
	Player* checkP = getWorld()->getCollidingPlayer(this);
	if (check != nullptr)
		check->applyBugSpray();
	if (checkP != nullptr)
		checkP->applyBugSpray();
}

void  WalkThruGoodie::doSomething()
{
	if (isDead())
		return;
	if (getLife() > 0)
		decreaseLife();
	if (getLife() == 0)
		setDead();
	Player* check = getWorld()->getCollidingPlayer(this);
	if (check != nullptr)
	{
		getWorld()->activatePlayerWalkThroughWalls();
		getWorld()->increaseScore(1000);
		setDead();
		getWorld()->playSound(SOUND_GOT_GOODIE);
	}
}

void  ExtraLifeGoodie::doSomething()
{
	if (isDead())
		return;
	if (getLife() > 0)
		decreaseLife();
	if (getLife() == 0)
		setDead();
	Player* check = getWorld()->getCollidingPlayer(this);
	if (check != nullptr)
	{
		getWorld()->incLives();
		getWorld()->increaseScore(1000);
		setDead();
		getWorld()->playSound(SOUND_GOT_GOODIE);
	}
}

void IncreaseSprayersGoodie::doSomething()
{
	if (isDead())
		return;
	if (getLife() > 0)
		decreaseLife();
	if (getLife() == 0)
		setDead();
	Player* check = getWorld()->getCollidingPlayer(this);
	if (check != nullptr)
	{
		getWorld()->increasePlayerSimultaneousSprayers();
		getWorld()->increaseScore(1000);
		setDead();
		getWorld()->playSound(SOUND_GOT_GOODIE);
	}
}

// Return a uniformly distributed random int from lowest to highest, inclusive
int randInt(int lowest, int highest)
{
    if (highest < lowest)
        swap(highest, lowest);
    return lowest + (rand() % (highest - lowest + 1));
}

Zumi::Zumi(StudentWorld* world, int imageID, int startX, int startY)
	:Actor(world, imageID, startX, startY) 
{
	int count = 0;
	m_currentDirection = 0;
}

void Zumi::setDirection(int direction)
{
	if (direction == -1)
	{
		srand(static_cast<unsigned int>(time(0)));
		m_currentDirection = randInt(0,3);
	}
	else
		m_currentDirection = direction;
}

bool Zumi::attemptMoveInDirection()
{
	Actor* actSpot;
	bool moved = true;
	switch (getDirection())
		{
			case 0:
				actSpot = getWorld()->objectAtSpotZumi(getX(),getY()+1);
				if (actSpot == nullptr && (getY() < (VIEW_HEIGHT-1)))
					moveTo(getX(),getY()+1);
				else
					if (!actSpot->blocksZumiAndSprayers() && (getY() < (VIEW_HEIGHT-1)))
						moveTo(getX(),getY()+1);
					else
						moved = false;
				break;
			case 1:
				actSpot = getWorld()->objectAtSpotZumi(getX()+1,getY());
				if (actSpot == nullptr && (getX() < (VIEW_WIDTH-1)))
					moveTo(getX()+1,getY());
				else
					if (!actSpot->blocksZumiAndSprayers() && (getX() < (VIEW_WIDTH-1)))
						moveTo(getX()+1,getY());
					else
						moved = false;
				break;
			case 2:
				actSpot = getWorld()->objectAtSpotZumi(getX(),getY()-1);

				if (actSpot == nullptr && (getY() > 0))
					moveTo(getX(),getY()-1);
				else
					if (!actSpot->blocksZumiAndSprayers() && (getY() > 0))
						moveTo(getX(),getY()-1);
					else
						moved = false;
				break;
			case 3:
				actSpot = getWorld()->objectAtSpotZumi(getX()-1, getY());
				if (actSpot == nullptr && (getX() > 0))
					moveTo(getX()-1, getY());
				else
					if (!actSpot->blocksZumiAndSprayers() && (getX() > 0))
						moveTo(getX()-1, getY());
					else
						moved = false;
				break;
		}
	return moved;
}

void Zumi::applyBugSpray()
{
	// Initialize the random number generator
    srand(static_cast<unsigned int>(time(0)));
	int randNum = randInt(0, 99);
	int probGoodie = getWorld()->getProbOfGoodieOverall();
	if (randNum < probGoodie)
	{
		int randNum2 = randInt(0,99);
		int probLife = getWorld()->getProbOfExtraLifeGoodie(), probWalk = getWorld()->getProbOfWalkThruGoodie(), probSpray = getWorld()->getProbOfMoreSprayersGoodie();
		
		if (probLife >= probWalk && probWalk >= probSpray)
		{
			if (randNum2 < probLife)
				getWorld()->addActor(new ExtraLifeGoodie(getWorld(), getX(), getY(), getWorld()->getGoodieLifetimeInTicks()));
			else if (randNum2 < (probLife+probWalk) && probWalk != 0)
				getWorld()->addActor(new WalkThruGoodie(getWorld(), getX(), getY(), getWorld()->getGoodieLifetimeInTicks()));
			else if (randNum2 < (probLife+probWalk+probSpray) && probSpray != 0)
				getWorld()->addActor(new IncreaseSprayersGoodie(getWorld(), getX(), getY(), getWorld()->getGoodieLifetimeInTicks()));
		}
		if (probLife >= probSpray && probSpray >= probWalk)
		{
			if (randNum2 < probLife)
				getWorld()->addActor(new ExtraLifeGoodie(getWorld(), getX(), getY(), getWorld()->getGoodieLifetimeInTicks()));
			else if (randNum2 < (probLife+probSpray) && probSpray != 0)
				getWorld()->addActor(new IncreaseSprayersGoodie(getWorld(), getX(), getY(), getWorld()->getGoodieLifetimeInTicks()));
			else if (randNum2 < (probLife+probWalk+probSpray) && probWalk != 0)
				getWorld()->addActor(new WalkThruGoodie(getWorld(), getX(), getY(), getWorld()->getGoodieLifetimeInTicks()));
		}
		if (probWalk >= probLife && probLife >= probSpray)
		{
			if (randNum2 < probWalk)
				getWorld()->addActor(new WalkThruGoodie(getWorld(), getX(), getY(), getWorld()->getGoodieLifetimeInTicks()));
			else if (randNum2 < (probLife+probWalk) && probLife != 0)
				getWorld()->addActor(new ExtraLifeGoodie(getWorld(), getX(), getY(), getWorld()->getGoodieLifetimeInTicks()));
			else if (randNum2 < (probLife+probWalk+probSpray) && probSpray != 0)
				getWorld()->addActor(new IncreaseSprayersGoodie(getWorld(), getX(), getY(), getWorld()->getGoodieLifetimeInTicks()));
		}
		if (probWalk >= probSpray && probSpray >= probLife)
		{
			if (randNum2 < probWalk)
				getWorld()->addActor(new WalkThruGoodie(getWorld(), getX(), getY(), getWorld()->getGoodieLifetimeInTicks()));
			else if (randNum2 < (probSpray+probWalk) && probSpray != 0)
				getWorld()->addActor(new IncreaseSprayersGoodie(getWorld(), getX(), getY(), getWorld()->getGoodieLifetimeInTicks()));
			else if (randNum2 < (probLife+probWalk+probSpray) && probLife != 0)
				getWorld()->addActor(new ExtraLifeGoodie(getWorld(), getX(), getY(), getWorld()->getGoodieLifetimeInTicks()));
		}
		if (probSpray >= probLife && probLife >= probWalk)
		{
			if (randNum2 < probSpray)
				getWorld()->addActor(new IncreaseSprayersGoodie(getWorld(), getX(), getY(), getWorld()->getGoodieLifetimeInTicks()));
			else if (randNum2 < (probSpray+probLife) && probLife != 0)
				getWorld()->addActor(new ExtraLifeGoodie(getWorld(), getX(), getY(), getWorld()->getGoodieLifetimeInTicks()));
			else if (randNum2 < (probLife+probWalk+probSpray) && probWalk != 0)
				getWorld()->addActor(new WalkThruGoodie(getWorld(), getX(), getY(), getWorld()->getGoodieLifetimeInTicks()));
		}
		if (probSpray >= probWalk && probWalk >= probLife)
		{
			if (randNum2 < probSpray)
				getWorld()->addActor(new IncreaseSprayersGoodie(getWorld(), getX(), getY(), getWorld()->getGoodieLifetimeInTicks()));
			else if (randNum2 < (probSpray+probWalk) && probWalk != 0)
				getWorld()->addActor(new WalkThruGoodie(getWorld(), getX(), getY(), getWorld()->getGoodieLifetimeInTicks()));
			else if (randNum2 < (probLife+probWalk+probSpray) && probLife != 0)
				getWorld()->addActor(new ExtraLifeGoodie(getWorld(), getX(), getY(), getWorld()->getGoodieLifetimeInTicks()));
		}
	}
	setDead();
	getWorld()->increaseScore(100);
	getWorld()->playSound(SOUND_ENEMY_DIE);
}

SimpleZumi::SimpleZumi(StudentWorld* world, int startX, int startY)
		:Zumi(world, IID_SIMPLE_ZUMI, startX, startY) 
{
	m_numTicksPerMove = getWorld()->getTicksPerSimpleZumiMove();
	m_currentTicks = m_numTicksPerMove;
	setDirection();
}

void SimpleZumi::doSomething()
{
	if (isDead())
		return;
	Player* check = getWorld()->getCollidingPlayer(this);
	if (check != nullptr)
	{
		check->setDead();
		return;
	}
	srand(static_cast<unsigned int>(time(0)));
	if (m_currentTicks <= 1)
	{
		if (!attemptMoveInDirection())
			setDirection();
		m_currentTicks = m_numTicksPerMove;
	}
	else 
		m_currentTicks--;
}

ComplexZumi::ComplexZumi(StudentWorld* world, int startX, int startY)
		:Zumi(world, IID_COMPLEX_ZUMI, startX, startY)
{
	m_numTicksPerMove = getWorld()->getTicksPerComplexZumiMove();
	m_currentTicks = m_numTicksPerMove;
	setDirection();
}


void ComplexZumi::doSomething()
{
	if (isDead())
		return;
	Player* check = getWorld()->getCollidingPlayer(this);
	if (check != nullptr)
	{
		check->setDead();
		return;
	}
	srand(static_cast<unsigned int>(time(0)));
	if (m_currentTicks <= 1)
	{
		/*int playerX = getWorld()->getPlayer()->getX();
		int playerY = getWorld()->getPlayer()->getY();
		int hDist = abs(playerX - getX());
		int vDist = abs(playerY - getY());
		int searchDistance = getWorld()->getComplexZumiSearchDistance();
		bool isPath = false;

		if (hDist < searchDistance && vDist < searchDistance)
		{
			queue<Coord> coordQueue;
			queue<Coord> coordQueue2;
			Coord init(getX(),getY());
			coordQueue.push(init);

			Coord a(getX()-1,getY());
			coordQueue2.push(a);
			a = Coord (getX(),getY()+1); 
			coordQueue2.push(a);
			a = Coord (getX()+1,getY());
			coordQueue2.push(a);
			a = Coord (getX(),getY()-1);

			Coord latest(getX(),getY());

			while (!coordQueue.empty())
			{
				Coord first = coordQueue.front();
				Coord first2 = coordQueue2.front();
				coordQueue.pop();
				if (!coordQueue2.empty())
					if (first.c() == first2.c() && first.r() == first2.r())
					{
						latest = first2;
						coordQueue2.pop();
					}
				if (first.c() == playerX && first.r() == playerY)
					isPath = true;
				if (getWorld()->objectAtSpot(first.r()-1,first.c()) == nullptr ||  !getWorld()->objectAtSpot(first.r()-1,first.c())->blocksZumiAndSprayers())
				{
					coordQueue.push(Coord (first.r()-1, first.c()));
				}
				if (getWorld()->objectAtSpot(first.r(),first.c()+1) == nullptr ||  !getWorld()->objectAtSpot(first.r(),first.c()+1)->blocksZumiAndSprayers())
				{
					coordQueue.push(Coord (first.r(), first.c()+1));
				}
				if (getWorld()->objectAtSpot(first.r()+1,first.c()) == nullptr ||  !getWorld()->objectAtSpot(first.r()+1,first.c())->blocksZumiAndSprayers())
				{
					coordQueue.push(Coord (first.r()+1, first.c()));
				}
				if (getWorld()->objectAtSpot(first.r(),first.c()-1) == nullptr ||  !getWorld()->objectAtSpot(first.r(),first.c()-1)->blocksZumiAndSprayers())
				{
					coordQueue.push(Coord (first.r(), first.c()-1));
				}
			}
			if (isPath)
			{
				int pathDirectionX = latest.c();
				int pathDirectionY = latest.r();

				if (getY() - pathDirectionY < 0)
					setDirection(0);
				if (getX() - pathDirectionX < 0)
					setDirection(1);
				if (getY() - pathDirectionY > 0)
					setDirection(2);
				if (getX() - pathDirectionX > 0)
					setDirection(3);
				attemptMoveInDirection();
			}
			else
			{
				if (!attemptMoveInDirection())
					setDirection();
				m_currentTicks = m_numTicksPerMove;
			}
		}
		else
		{*/
			if (!attemptMoveInDirection())
				setDirection();
			m_currentTicks = m_numTicksPerMove;
		//}
	}
	else 
		m_currentTicks--;
}

void ComplexZumi::applyBugSpray()
{
	Zumi::applyBugSpray();
	getWorld()->increaseScore(400);
}
