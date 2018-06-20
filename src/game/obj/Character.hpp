#ifndef _Character_
#define _Character_

#include "../obj/Object.hpp"
#include "../Collision.hpp"

#include "../player/Weapon.hpp"

using namespace sf;

class Character: public Object {

public:

	float health;

	CollisionBox walkBox, bodyBox;

	Weapon *weapon;

	Character(Vector2f position, float healthParam):
		Object(position),
		health(healthParam)
	{}

	virtual ~Character() {}

	virtual void hit(Vector2f direction) = 0;
	
};

#endif