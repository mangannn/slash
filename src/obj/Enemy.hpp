#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "../Object.hpp"
#include "../Functions.hpp"
#include "../Resources.hpp"
#include "../Collision.hpp"

#include <math.h>


class Enemy: public Object {

public:

	sf::Sprite sprite;

	Vector2f vel;

	CollisionBox box;

	float timer = 0;
	int state = 0;

	float wobble = 0;

	Enemy(Vector2f position):
		Object(position)
	{

		sf::Texture *texture = getTexture("media/images/enemy.png");
		sprite.setTexture(*texture);
		sprite.setOrigin(sf::Vector2f((float)texture->getSize().x / 2.0f, (float)texture->getSize().y));

		sprite.setColor(RANDOM_COLOR);

		box = CollisionBox(&pos, Vector2f(0,-(float)texture->getSize().y / 2.0f), (float)texture->getSize().x / 2.0f);
	}

	virtual ~Enemy() {}

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
};

#endif
