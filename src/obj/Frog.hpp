#ifndef _Frog_
#define _Frog_

#include "../Object.hpp"
#include "../Functions.hpp"
#include "../Resources.hpp"
#include "../Collision.hpp"


#include "../World.hpp"
#include "Explotion.hpp"

#include <math.h>


class Frog: public Enemy {

public:

	sf::Sprite sprite;

	Vector2f vel;

	float timer = 0;
	int state = 0;

	float wobble = 0;

	Frog(Vector2f position):
		Enemy(position)
	{

		sf::Texture *texture = Resources::getTexture("media/images/frog.png");
		sprite.setTexture(*texture);
		sprite.setOrigin(sf::Vector2f((float)texture->getSize().x / 2.0f, (float)texture->getSize().y));

		int a = 120;
		sprite.setColor(Color(255 - a * RANDOM, 255 - a * RANDOM, 255 - a * RANDOM));
		//sprite.setColor(RANDOM_COLOR);

		box = CollisionBox(&pos, Vector2f(0,-(float)texture->getSize().y / 2.0f), (float)texture->getSize().x / 2.0f);
	}

	virtual ~Frog() {}

	virtual void update(float elapsedTime) {

		timer += elapsedTime;
		switch (state) {
			case 0: {
				if (timer > 1) {
					timer -= 1;
					float dir = RANDOM * M_PI * 2;
					vel = 20.f * Vector2f(cos(dir),sin(dir));
					state = 1;
				}
			} break;
			case 1: {

				wobble = 20 * sin(timer * 10);

				if (timer > 3) {
					timer -= 3;
					vel = Vector2f(0,0);
					wobble = 0;
					state = 0;
				}
			} break;
		}

		pos += vel * elapsedTime;
	}

	virtual void draw(RenderTarget *target, RenderTarget *monitor) {

		sprite.setRotation(wobble);
		sprite.setPosition(pos);
		target->draw(sprite);

		//box.draw(target);
	}

	virtual void kill() {

		World::add(new Explotion(pos + Vector2f(0, 20)));
		World::remove(this);
	}
};

#endif
