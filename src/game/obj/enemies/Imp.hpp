#ifndef _Imp_
#define _Imp_

#include "../Character.hpp"
#include "../../Functions.hpp"
#include "../../Resources.hpp"
#include "../../Collision.hpp"


#include "../../World.hpp"

#include "../../player/Spear.hpp"

#include <math.h>


class Imp: public Character {

public:

	Animation ani;

	Vector2f velocity;

	float direction = 0;

	Imp(Vector2f position):
		Character(position, 100),
		ani(&pos, Vector2f(0, 0), "media/animation/Imp/empty", 8)
	{
		bodyBox = CollisionBox(&pos, Vector2f(0,-32), 12);
		walkBox = CollisionBox(&pos, Vector2f(0,-26), 6);

		weapon = new Spear(&pos);
	}

	virtual ~Imp() {
		delete weapon;
	}

	virtual void update(float elapsedTime) {

		if (weapon->isBusy()) {
			velocity = Vector2f(0,0);
		} else {
			Vector2f focus = World::players->at(0)->pos;
			Vector2f diff = focus - pos;
			float distToFocus = size(diff);

			float goalDirection = atan2(diff.y, diff.x);


			const float turn_speed = 3.f;

			direction = towardsAngle(direction, goalDirection, turn_speed * elapsedTime);

			Vector2f dirVec = Vector2f(cos(direction), sin(direction));

			const float velocityAmount = 60.f;

			if (distToFocus > 100) {
				velocity = velocityAmount * dirVec;
			} else if (distToFocus < 80) {
				velocity = -velocityAmount * 0.5f * dirVec;
			} else {
				velocity = Vector2f(0,0);
			}

			ani.setFrame(discreteDirectionFromAngle(direction, 8));
		}

		pos += velocity * elapsedTime;

		weapon->update(elapsedTime, direction);
	}

	virtual void draw(RenderTarget *target) {

		ani.draw(target);

		weapon->draw(target);

		bodyBox.draw(target);
		walkBox.draw(target);
	}

	virtual void hit(Vector2f direction) {

		World::add(new Explotion(pos));
		//World::remove(this);
		pos = Vector2f(0,0);
	}
};

#endif
