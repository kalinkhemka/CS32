#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"
#include "GameWorld.h"
#include "Level.h"
class StudentWorld;

class Actor: public GraphObject
{
public:
	Actor(StudentWorld* world, int imageID, int startX, int startY) :GraphObject(imageID, startX, startY) 
	{
		m_isDead = false;
		setVisible(true);
		m_thisWorld = world;
	}
	virtual ~Actor()
	{
		setVisible(false);
	}
	virtual void  doSomething() = 0;
	virtual void setDead()
	{
		m_isDead = true;
		setVisible(false);
	}
	bool isDead() const
	{
		return m_isDead;
	}
	StudentWorld* getWorld() const
	{
		return m_thisWorld;
	}
	// Take action when this actor is co-located with bug spray
	virtual void applyBugSpray(){}
    // Does this actor block the player from moving onto it?
    virtual bool blocksPlayer() const{return true;}
    // Does a Player on this actor die?
    virtual bool colocationKillsPlayer() const{return false;}    
    // Does this actor block a Zumi from being moved onto it or a
    // bug sprayer from being dropped onto it?
    virtual bool blocksZumiAndSprayers() const{return false;}
    // Can bug spray be dropped onto this actor?
    virtual bool allowsSprayToBeDropped() const{return true;}
    // Can bug spray be sprayed beyond this actor?
    virtual bool allowsSprayToPass() const{return true;}
	virtual bool isZumi() const {return false;}
private:
	bool m_isDead;
	StudentWorld* m_thisWorld;

};

class Player: public Actor
{
public:
	Player(StudentWorld* world, int startX, int startY)
		:Actor(world, IID_PLAYER, startX, startY)
	{
		m_maxBugSprayers = 2;
		m_activeBugSprayers = 0;
		m_canWalkThroughWalls = false;
		m_wallTime = 0;
		m_sprayerTime = 0;
	}
	~Player(){}
	virtual void setDead();
	virtual void  doSomething();
	virtual void applyBugSpray()
	{
		setDead();
	}
    // Turn on ability to walk through destroyable bricks
    void activateWalkThroughWalls(unsigned int lifetime)
	{
		m_canWalkThroughWalls = true;
		m_wallTime = lifetime;
	}
    // Can player walk through destroyable bricks?
    bool canWalkThroughWalls() const
	{
		return m_canWalkThroughWalls;
	}
    // Increase the maximum simultaneous sprayers that can be dropped
    void increaseSimultaneousSprayers(unsigned int max, unsigned int lifetime)
	{
		m_maxBugSprayers = max;
		m_sprayerTime = lifetime;
	}
	void reduceActiveSprayers()
	{
		m_activeBugSprayers--;
	}

private:
	int m_maxBugSprayers, m_activeBugSprayers;
	bool m_canWalkThroughWalls;
	int m_wallTime, m_sprayerTime;
};


class Brick: public Actor
{
public:
	Brick(StudentWorld* world, int imageID, int startX, int startY)
		:Actor(world, imageID, startX, startY) {}
	virtual ~Brick() {}
	virtual void doSomething(){}
    virtual bool blocksZumiAndSprayers() const{return true;}
    virtual bool allowsSprayToPass() const{return false;}
private:
	
};

class PermaBrick: public Brick
{
public:
	PermaBrick(StudentWorld* world, int startX, int startY)
		:Brick(world, IID_PERMA_BRICK, startX, startY) {}
	~PermaBrick() {}
	virtual bool allowsSprayToBeDropped() const{return false;}
private:

};

class DestBrick: public Brick
{
public:
	DestBrick(StudentWorld* world, int startX, int startY)
		:Brick(world, IID_DESTROYABLE_BRICK, startX, startY) {}
	~DestBrick() {}
	virtual void applyBugSpray()
	{
		setDead();
		setVisible(false);
	}
    virtual bool colocationKillsPlayer() const;
	virtual bool blocksPlayer() const;
private:

};

class Exit: public Actor
{
public:
	Exit(StudentWorld* world, int startX, int startY)
		:Actor(world, IID_EXIT, startX, startY) {setVisible(false);}
	virtual ~Exit() {}
	virtual void doSomething();
	virtual bool blocksPlayer() const{return false;}
private:
};

class TimedLifetimeActor : public Actor
{
public:
    TimedLifetimeActor(StudentWorld* world, int imageID, int startX, int startY, unsigned int lifetime)
		:Actor(world, imageID, startX, startY) 
	{
		m_lifetime = lifetime;
	}
	virtual bool blocksPlayer() const{return false;}
	unsigned int getLife() const
	{
		return m_lifetime;
	}
	void decreaseLife()
	{
		m_lifetime--;
	}
	void expireImmediately()
	{
		m_lifetime = 0;
	}
private:
	int m_lifetime;
};

class BugSprayer : public TimedLifetimeActor
{
public:
    BugSprayer(StudentWorld* world, int startX, int startY)
		:TimedLifetimeActor(world, IID_BUGSPRAYER, startX, startY, 40) {}
    virtual void doSomething();
    virtual void applyBugSpray()
	{
		expireImmediately();
		doSomething();
	}
	virtual void setDead();
    virtual bool blocksZumiAndSprayers() const{return true;}
};

class BugSpray : public TimedLifetimeActor
{
public:
    BugSpray(StudentWorld* world, int startX, int startY)
		:TimedLifetimeActor(world, IID_BUGSPRAY, startX, startY, 3) {}
    virtual void doSomething();
};

class Goodie: public TimedLifetimeActor
{
public:
	Goodie(StudentWorld* world, int imageID, int startX, int startY, int lifetime)
		:TimedLifetimeActor(world, imageID, startX, startY, lifetime) {}
	virtual ~Goodie() {}
private:
};

class WalkThruGoodie: public Goodie
{
public:
	WalkThruGoodie(StudentWorld* world, int startX, int startY, int lifetime)
		:Goodie(world, IID_WALK_THRU_GOODIE, startX, startY, lifetime) {}
	~WalkThruGoodie() {}
	virtual void  doSomething();

private:
	
};

class ExtraLifeGoodie: public Goodie
{
public:
	ExtraLifeGoodie(StudentWorld* world, int startX, int startY, int lifetime)
		:Goodie(world, IID_EXTRA_LIFE_GOODIE, startX, startY, lifetime) {}
	~ExtraLifeGoodie() {}
	virtual void  doSomething();

private:
	
};
class IncreaseSprayersGoodie: public Goodie
{
public:
	IncreaseSprayersGoodie(StudentWorld* world, int startX, int startY, int lifetime)
		:Goodie(world, IID_INCREASE_SIMULTANEOUS_SPRAYER_GOODIE, startX, startY, lifetime) {}
	~IncreaseSprayersGoodie() {}
	virtual void  doSomething();
private:
};

class Zumi: public Actor
{
public:
	Zumi(StudentWorld* world, int imageID, int startX, int startY);
	virtual ~Zumi() {}
	virtual void applyBugSpray();
    virtual bool blocksPlayer() const{return false;}
    virtual bool colocationKillsPlayer() const{return true;}
	virtual bool isZumi() const {return true;}
	virtual void setDirection(int direction = -1);
	int getDirection() const {return m_currentDirection;}
	bool attemptMoveInDirection();

private:
	int m_currentDirection;
};

class SimpleZumi: public Zumi
{
public:
	SimpleZumi(StudentWorld* world, int startX, int startY);
	~SimpleZumi() {}
	virtual void  doSomething();

private:
	int m_numTicksPerMove;
	int m_currentTicks;
};


class Coord
{
    public:
    Coord(int cc, int rr) : m_c(cc), m_r(rr) {}
    int c() const { return m_c; }
	int r() const { return m_r; }
    private:
	int m_c;
    int m_r;
};

class ComplexZumi: public Zumi
{
public:
	ComplexZumi(StudentWorld* world, int startX, int startY);
	~ComplexZumi() {}
	virtual void  doSomething();
	virtual void applyBugSpray();

private:
	int m_numTicksPerMove;
	int m_currentTicks;
};

#endif // ACTOR_H_
