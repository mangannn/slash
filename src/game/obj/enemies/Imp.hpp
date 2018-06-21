#ifndef _Imp_
#define _Imp_

#include "../Character.hpp"
#include "../../Functions.hpp"
#include "../../Resources.hpp"
#include "../../Collision.hpp"


#include "../../World.hpp"

#include "../../player/Spear.hpp"
#include "../../player/Sword.hpp"

#include <math.h>


class Imp: public Character {

public:

	Animation ani;

	float velocity;

	float direction = 0;

	float timer = 0;
	float nextTime = 10.f;

	Imp(Vector2f position, int type):
		Character(position, 10),
		ani(&pos, Vector2f(0, 0), "media/animation/Imp/empty", 8)
	{
		bodyBox = CollisionBox(&pos, Vector2f(0,0), 12);
		walkBox = CollisionBox(&pos, Vector2f(0,6), 8);

		if (type == 0) {
			weapon = new Spear(&pos);
		} else {
			weapon = new Sword(&pos);
		}
	}

	virtual ~Imp() {
		delete weapon;
	}

	virtual void update(float elapsedTime) {

		timer += elapsedTime;

		if (weapon->isBusy()) {
			velocity = weapon->getCharacterVelocity();
		} else {
			Vector2f focus = World::players->at(0)->pos;
			Vector2f diff = focus - pos;
			float distToFocus = size(diff);

			float goalDirection = atan2(diff.y, diff.x);


			const float turn_speed = 2.5f;

			direction = towardsAngle(direction, goalDirection, turn_speed * elapsedTime);

			const float velocityAmount = 60.f;

			if (distToFocus > 300) {
				velocity = 0;
			} else if (distToFocus > 100) {
				velocity = velocityAmount;
			} else if (distToFocus < 80) {
				velocity = -velocityAmount * 0.5f;

				if (timer > nextTime) {
					timer = 0;
					nextTime = 4 + (rand() % 10);
					weapon->setAction(Weapon::Action::Slash);
				}
			} else {
				velocity = 0;

				if (timer > nextTime) {
					timer = 0;
					nextTime = 4 + (rand() % 10);
					weapon->setAction(Weapon::Action::Stab);
				}
			}

			ani.setFrame(discreteDirectionFromAngle(direction, 8));
		}

		pos += (velocity * elapsedTime) * Vector2f(cos(direction), sin(direction));

		weapon->update(elapsedTime, direction);
	}

	virtual void draw(RenderTarget *target) {

		ani.draw(target);
		weapon->draw(target);
	}

	virtual void hit(Vector2f dir) {

		direction = atan2(-dir.y, -dir.x);

		std::cout << "imp hurt\n";

		hp--;
		weapon->setAction(Weapon::Action::GetHit);

		if (hp <= 0) {
			World::add(new Explotion(pos));
			World::remove(this);
		}
	}
};

#endif
