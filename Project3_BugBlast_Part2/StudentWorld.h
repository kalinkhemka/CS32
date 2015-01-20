#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include "Level.h"
#include <list>
#include <iostream> // defines the overloads of the << operator
#include <sstream>  // defines the type std::ostringstream
#include <iomanip> // defines the manipulator setw

class StudentWorld : public GameWorld
{
public:
	StudentWorld()
	{
		
	}
	virtual ~StudentWorld()
	{
		while (!m_actors.empty())
		{
			delete m_actors.back();
			m_actors.pop_back();
		}
		delete m_player;
	}

	//void setDisplayText();

	virtual int init()
    {
		m_levelFinished = false;
		std::ostringstream oss;
		int currentLevel = getLevel();
		oss.fill('0');
		oss << "level" << std::setw(2) << currentLevel << ".dat";
		std::string	curLevel = oss.str();
		//curLevel = "level02.dat";
		Level::LoadResult	result	=	m_Level.loadLevel(curLevel);
		if ((result == Level::load_fail_file_not_found || result	== Level::load_fail_bad_format) && currentLevel == 0)	
			return	GWSTATUS_NO_FIRST_LEVEL;
		if (result	== Level::load_fail_bad_format)	
			return	GWSTATUS_LEVEL_ERROR;
		if (result == Level::load_fail_file_not_found)	
			return	GWSTATUS_PLAYER_WON;
		m_levelBonus = m_Level.getOptionValue(optionLevelBonus);
		for (int i = 0; i < VIEW_HEIGHT; i++)
		{
			for (int j = 0; j < VIEW_WIDTH; j++)
			{
				Level::MazeEntry newME = m_Level.getContentsOf(i,j);
				switch(newME)
				{
					case Level::exit:
						m_actors.push_back(new Exit(this, i,j));
						break;
					case Level::player:
						m_player = new Player(this, i,j);
						break;
					case Level::simple_zumi:
						m_actors.push_back(new SimpleZumi(this, i,j));
						break;
					case Level::complex_zumi:
						m_actors.push_back(new ComplexZumi(this, i,j));
						break;
					case Level::perma_brick:
						m_actors.push_back(new PermaBrick(this, i,j));
						break;
					case Level::destroyable_brick:
						m_actors.push_back(new DestBrick(this, i,j));
						break;
				}
			}
		}
		setDisplayText();
		return GWSTATUS_CONTINUE_GAME;
    }

	void setDisplayText()
	{
		std::ostringstream oss;
		int currentLevel = getLevel();
		oss.fill('0');
		oss << "Score: " << std::setw(8) << getScore() << "  Level: " << std::setw(2) << getLevel() << "  Lives: " << std::setw(3) << getLives();
		oss.fill(' ');
		oss << "  Bonus: " << std::setw(6) << m_levelBonus;
		std::string	text = oss.str();

		setGameStatText(text);
	}


	virtual int move()
    {
		setDisplayText();
		std::list<Actor*>::iterator it = m_actors.begin();
		m_player->doSomething();
		if (m_levelFinished == true)
		{
			increaseScore(m_levelBonus);
			return GWSTATUS_FINISHED_LEVEL;
		}
		while (it != m_actors.end())
		{
			(*it)->doSomething();
			if (m_player->isDead())
			{
				decLives();
				return GWSTATUS_PLAYER_DIED; 
			}
			it++;
		}
		it = m_actors.begin();
		while (it != m_actors.end())
		{
			if ((*it)->isDead())
			{
				delete *it;
				m_actors.erase(it);
				it = m_actors.begin();
			}
			else
				it++;

		}
		if (m_levelBonus > 0)
			m_levelBonus--;
		return GWSTATUS_CONTINUE_GAME;
    }

	virtual void cleanUp()
    {
		while (!m_actors.empty())
		{
			delete m_actors.back();
			m_actors.pop_back();
		}
		delete m_player;
    }

	Actor* objectAtSpot(int x, int y) const
	{
		std::list<Actor*>::const_iterator it = m_actors.begin();
		while (it != m_actors.end())
		{
			if ((*it)->getX() == x && (*it)->getY() == y)
				return *it;
			it++;
		}
		return nullptr;
	}

	Actor* objectAtSpotZumi(int x, int y) const
	{
		std::list<Actor*>::const_iterator it = m_actors.begin();
		while (it != m_actors.end())
		{
			if ((*it)->getX() == x && (*it)->getY() == y && !(*it)->isZumi())
				return *it;
			it++;
		}
		return nullptr;
	}

	 // Get the player's coordinates
    void getPlayerLocation(int& x, int& y) const
	{
		x = m_player->getX();
		y = m_player->getY();
	}
	Player* getPlayer() const
	{
		return m_player;
	}



      // Get a pointer to the player if at the same location as the actor,
      // otherwise nullptr
    Player* getCollidingPlayer(Actor* a) const
	{
		if (a->getX() == m_player->getX() && a->getY() == m_player->getY()) 
			return m_player;
		return nullptr;
	}

      // Turn on player's ability to walk through destroyable bricks
    void activatePlayerWalkThroughWalls()
	{

		m_player->activateWalkThroughWalls(getWalkThruLifetimeTicks());
	}

      // Can player walk through destroyable bricks?
    bool playerCanWalkThroughWalls() const
	{
		if (m_player->canWalkThroughWalls())
			return true;
		return false;
	}

      // Increase the maximum simultaneous sprayers that can be dropped
    void increasePlayerSimultaneousSprayers()
	{
		m_player->increaseSimultaneousSprayers(getMaxBoostedSprayers(), getBoostedSprayerLifetimeTicks());
	}
    
    // Are there any living Zumi left?
    bool anyLiveZumi() const
	{
		std::list<Actor*>::const_iterator it = m_actors.begin();
		while (it != m_actors.end())
		{
			if ((*it)->isZumi())
				return true;
			it++;
		}
		return false;
	}
    
      // Indicate that the player has finished the level
    void setLevelFinished()
	{
		m_levelFinished = true;
	}

      // Add an actor to the world
    void addActor(Actor* a)
	{
		m_actors.push_back(a);
	}

      // Report the value of the level data file options
    unsigned int getProbOfGoodieOverall() const
	{
		return m_Level.getOptionValue(optionProbOfGoodieOverall);
	}
    unsigned int getProbOfExtraLifeGoodie() const
	{
		return m_Level.getOptionValue(optionProbOfExtraLifeGoodie);
	}
    unsigned int getProbOfWalkThruGoodie() const	
	{
		return m_Level.getOptionValue(optionProbOfWalkThruGoodie);
	}
    unsigned int getProbOfMoreSprayersGoodie() const
	{
		return m_Level.getOptionValue(optionProbOfMoreSprayersGoodie);
	}
    unsigned int getTicksPerSimpleZumiMove() const
	{
		return m_Level.getOptionValue(optionTicksPerSimpleZumiMove);
	}
    unsigned int getTicksPerComplexZumiMove() const
	{
		return m_Level.getOptionValue(optionTicksPerComplexZumiMove);
	}
    unsigned int getGoodieLifetimeInTicks() const
	{
		return m_Level.getOptionValue(optionGoodieLifetimeInTicks);
	}
	unsigned int getMaxBoostedSprayers() const
	{
		return m_Level.getOptionValue(optionMaxBoostedSprayers);
	}
	unsigned int getWalkThruLifetimeTicks() const
	{
		return m_Level.getOptionValue(optionWalkThruLifetimeTicks);
	}
	unsigned int getBoostedSprayerLifetimeTicks() const
	{
		return m_Level.getOptionValue(optionBoostedSprayerLifetimeTicks);
	}
    unsigned int getComplexZumiSearchDistance() const
	{
		return m_Level.getOptionValue(optionComplexZumiSearchDistance);
	}

private:
	std::list<Actor*> m_actors;
	Player* m_player;
	Level m_Level;
	bool m_levelFinished;
	int m_levelBonus;
};

#endif // STUDENTWORLD_H_
