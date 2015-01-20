#include "Actor.h"
#include "StudentWorld.h"

void  Player::doSomething()
{
	if (isDead())
		return;
	int ch;
	if (getWorld()->getKey(ch))
	{
		switch(ch)
		{
			case KEY_PRESS_LEFT:
				if (!getWorld()->objectAtSpot(getX()-1, getY()) && (getX() > 0))
					moveTo(getX()-1, getY());
				break;
			case KEY_PRESS_RIGHT:
				if (!getWorld()->objectAtSpot(getX()+1,getY()) && (getX() < (VIEW_WIDTH-1)))
					moveTo(getX()+1,getY());
				break;
			case KEY_PRESS_DOWN:
				if (!getWorld()->objectAtSpot(getX(),getY()-1) && (getY() > 0))
					moveTo(getX(),getY()-1);
				break;
			case KEY_PRESS_UP:
				if (!getWorld()->objectAtSpot(getX(),getY()+1) && (getY() < (VIEW_HEIGHT-1)))
					moveTo(getX(),getY()+1);
				break;
			case KEY_PRESS_SPACE:
				break;
			default:
				break;
		}
	}

}