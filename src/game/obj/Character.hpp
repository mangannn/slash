#ifndef _Character_
#define _Character_

#include "../obj/Object.hpp"
#include "../Collision.hpp"

#include "../player/Weapon.hpp"

using namespace sf;

class Character: public Object {

public:

	float hp;

	CollisionBox walkBox, bodyBox;

	Weapon *weapon;

	Character(Vector2f position, float health):
		Object(position),
		hp(health)
	{}

	virtual ~Character() {}

	virtual void hit(Vector2f direction) = 0;
	
#ifdef DEBUG
	virtual void drawDebug(RenderTarget *target) {

		bodyBox.drawDebug(target);
		walkBox.drawDebug(target);

		weapon->drawDebug(target);
	}
#endif
	
};

#endif
