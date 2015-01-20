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
		m_numActors = 0;
	}
	~StudentWorld()
	{
		while (!m_actors.empty())
		{
			delete m_actors.back();
			m_actors.pop_back();
		}
		delete m_player;
	}

	virtual int init()
    {
		std::ostringstream oss;
		int currentLevel = getLevel();
		oss.fill('0');
		oss << "level" << std::setw(2) << currentLevel << ".dat";
		std::string	curLevel = oss.str();
		Level::LoadResult	result	=	m_Level.loadLevel(curLevel);
		if ((result == Level::load_fail_file_not_found || result	== Level::load_fail_bad_format) && currentLevel == 0)	
			return	GWSTATUS_NO_FIRST_LEVEL;
		if (result	== Level::load_fail_bad_format)	
			return	GWSTATUS_LEVEL_ERROR;
		if (result == Level::load_fail_file_not_found)	
			return	GWSTATUS_PLAYER_WON;

		for (int i = 0; i < VIEW_HEIGHT; i++)
		{
			for (int j = 0; j < VIEW_WIDTH; j++)
			{
				Level::MazeEntry newME = m_Level.getContentsOf(i,j);
				switch(newME)
				{
					case Level::empty:
						break;
					case Level::exit:
						break;
					case Level::player:
						m_player = new Player(i,j);
						m_player->setWorld(this);
						break;
					case Level::simple_zumi:
						break;
					case Level::complex_zumi:
						break;
					case Level::perma_brick:
						m_actors.push_back(new PermaBrick(i,j));
						m_numActors++;
						break;
					case Level::destroyable_brick:
						m_actors.push_back(new DestBrick(i,j));
						m_numActors++;
						break;
				}
			}
		}
		return GWSTATUS_CONTINUE_GAME;
    }

	virtual int move()
    {
		std::list<Actor*>::iterator it = m_actors.begin();
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
		m_player->doSomething();
		if (m_player->isDead())
		{
			decLives();
			return GWSTATUS_PLAYER_DIED; 
		}
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

	bool objectAtSpot(int x, int y) const
	{
		std::list<Actor*>::const_iterator it = m_actors.begin();
		while (it != m_actors.end())
		{
			if ((*it)->getX() == x && (*it)->getY() == y)
				return true;
			it++;
		}
		return false;
	}

private:
	std::list<Actor*> m_actors;
	Actor* m_player;
	int m_numActors;
	Level m_Level;


};

#endif // STUDENTWORLD_H_
