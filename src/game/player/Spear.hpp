#ifndef _Spear_
#define _Spear_

#include <math.h>

#include "Weapon.hpp"

#include "WeaponMove.hpp"

#include "../Collision.hpp"
#include "../Functions.hpp"

#include "../Animation.hpp"

#include "SwordGraphics.hpp"


#include "../obj/graphicaleffects/Flash.hpp"

using namespace sf;

class Spear: public Weapon {

public:

	Sprite spearSprite;

	CollisionBox spearBox;

	enum class State {
		Point,
		Hurt
	};

	State state = State::Point;

	float spearDir = 0;

	float timer = 0;

	Vector2f tip;

	Spear(Vector2f *offset):
		Weapon(offset),

		spearBox(offset, Vector2f(0, 0), 4)
	{

		Texture *texture = Resources::getTexture("media/images/weapons/spear.png");
		spearSprite.setTexture(*texture);
		spearSprite.setOrigin(sf::Vector2f((float)texture->getSize().x / 2.0f, (float)texture->getSize().y / 2.0f));
	}

	virtual ~Spear() {}
	
	virtual void update(float elapsedTime, float characterDirection) {

		timer += elapsedTime;

		if (state == State::Hurt) {
			const float a = 0.2f;

			spearBox.pos = Vector2f(0, -32);
			if (timer < a) {
				spearDir = characterDirection + approach(0, M_PI_2, timer/a);
			} else {
				spearDir = characterDirection + approach(M_PI_2, 0, (timer-a)/a);
				if (timer > 2.f*a) {
					state = State::Point;
				}
			}
		} else if (state == State::Point) {

			spearDir = characterDirection;

			tip = 56.f * Vector2f(cos(spearDir), sin(spearDir));

			spearBox.pos = Vector2f(0, -32) + tip;
		}
	}

	virtual void draw(RenderTarget *target) {

		spearSprite.setRotation(90.f + spearDir * 180.f / M_PI);
		spearSprite.setPosition(*offset + Vector2f(0,-32));
		target->draw(spearSprite);

		spearBox.draw(target);
	}

	virtual bool isBehind() {
		return (tip.x + tip.y <= 0);
	}

	virtual void setAction(Action a) {}

	virtual bool isHitCircle(CollisionBox c) {
		return CollisionBox::check(spearBox, c);
	}

	virtual bool isBusy() {
		return (state != State::Point);
	}

	virtual float getCharacterVelocity() {
		return 0;
	}

	virtual CollisionBox getParayBox() {
		return spearBox;
	}
	virtual void paray() {
		state = State::Hurt;
		timer = 0;
	}
};

#endif
