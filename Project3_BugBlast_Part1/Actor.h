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
	Actor(int imageID, int startX, int startY) :GraphObject(imageID, startX, startY) 
	{
		m_isDead = false;
		setVisible(true);
		m_thisWorld = nullptr;
	}
	virtual ~Actor()
	{
		setVisible(false);
	}
	virtual void  doSomething() = 0;
	void setDead()
	{
		m_isDead = true;
	}
	bool isDead() const
	{
		return m_isDead;
	}
	void setWorld(StudentWorld* world)
	{
		m_thisWorld = world;
	}
	StudentWorld* getWorld() const
	{
		return m_thisWorld;
	}
private:
	bool m_isDead;
	StudentWorld* m_thisWorld;

};

class Player: public Actor
{
public:
	Player(int startX, int startY)
		:Actor(IID_PLAYER, startX, startY)
	{
		m_activeBugSprayers = 0;
		canWalkThroughBricks = false;
	}
	~Player()
	{
	}

	void  doSomething();

private:
	int m_activeBugSprayers;
	bool canWalkThroughBricks;
};


class Brick: public Actor
{
public:
	Brick(int imageID, int startX, int startY)
		:Actor(imageID, startX, startY) {}
	virtual ~Brick() {}
	void  doSomething() {}

private:
	
};

class PermaBrick: public Brick
{
public:
	PermaBrick(int startX, int startY)
		:Brick(IID_PERMA_BRICK, startX, startY) {}
	~PermaBrick() {}
private:

};
class DestBrick: public Brick
{
public:
	DestBrick(int startX, int startY)
		:Brick(IID_DESTROYABLE_BRICK, startX, startY) {}
	~DestBrick() {}
private:

};

#endif // ACTOR_H_
