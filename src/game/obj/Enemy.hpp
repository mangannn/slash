#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "Object.hpp"
#include "../Collision.hpp"


class Enemy: public Object {

public:

	CollisionBox box;

	Enemy(Vector2f position):
		Object(position)
	{
	}

	virtual ~Enemy() {}

	virtual void kill() = 0;
};

#endif
