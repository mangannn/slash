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

	Sprite sprite;

	CollisionBox spearBox;

	enum class State {
		Point,
		DrawBack,
		Hurt,
		ParriedLeft,
		ParriedRight,
		Poke
	};

	State state = State::Point;

	float spearDir = 0;
	Vector2f spearDirVec;

	float spearDirVel = 0;

	Vector2f tip, ori;

	const float weaponLength = 56.f;

	WeaponMove currentMove = WeaponMove(0,0,0,0,0,1);

	Spear(Vector2f *offset):
		Weapon(offset),

		spearBox(offset, Vector2f(0, 0), 4)
	{

		Texture *texture = Resources::getTexture("media/images/weapons/spear.png");
		sprite.setTexture(*texture);
		sprite.setOrigin(sf::Vector2f((float)texture->getSize().x / 2.0f, (float)texture->getSize().y / 2.0f));

		setState(State::Point);
	}

	virtual ~Spear() {}
	
	virtual void update(float elapsedTime, float characterDirection) {

		if (state != State::Point) {
			if (currentMove.update(elapsedTime)) {
				if (state == State::DrawBack) {
					setState(State::Point);
				} else {
					setState(State::DrawBack);
				}
			}
		}

		spearDir = currentMove.getDirection() + characterDirection;
		spearDirVec = Vector2f(cos(spearDir), sin(spearDir));

		float dist = currentMove.getLength();
		tip = dist * spearDirVec;
		ori = (dist - weaponLength) * spearDirVec;

		spearBox.pos = tip;
	}

	virtual void draw(RenderTarget *target) {

		sprite.setRotation(90.f + spearDir * 180.f / M_PI);
		sprite.setPosition(*offset + ori);
		target->draw(sprite);
	}

#ifdef DEBUG
	virtual void drawDebug(RenderTarget *target) {
		spearBox.drawDebug(target);
	}
#endif

	void setState(State s) {

		if (s == State::DrawBack) {
			currentMove = WeaponMove(currentMove.reachEnd, weaponLength, currentMove.dirEnd, 0, 0, 0.2);
		} else if (s == State::Hurt) {
			currentMove = WeaponMove(weaponLength, weaponLength*0.7f, 0, M_PI_2, -200, 0.3);
		} else if (s == State::ParriedRight) {
			currentMove = WeaponMove(weaponLength, weaponLength*1.2f, 0, M_PI_2, -100, 0.4);
		} else if (s == State::ParriedLeft) {
			currentMove = WeaponMove(weaponLength, weaponLength*1.2f, 0, -M_PI_2, -100, 0.4);
		} else if (s == State::Poke) {
			currentMove = WeaponMove(weaponLength*0.8, weaponLength*1.4f, 0, 0, 0, 0.2);
		} else if (s == State::Point) {
			currentMove = WeaponMove(weaponLength, weaponLength, 0, 0, 0, 1);
		}

		state = s;
	}

	virtual bool isBehind() {
		return (tip.x + tip.y <= 0);
	}

	virtual void setAction(Action a) {
		if (a == Action::GetHit) {
			setState(State::Hurt);
		} else {
			if (state == State::Point) {
				setState(State::Poke);
			}
		}
	}

	virtual bool isHitCircle(CollisionBox c, Vector2f *returnDirVec) {
		*returnDirVec = spearDirVec;
		return CollisionBox::check(spearBox, c);
	}

	virtual bool isBusy() {
		return (state != State::Point);
	}

	virtual float getCharacterVelocity() {
		return currentMove.getVelocity();
	}

	virtual bool canBeParried() {
		return !(state == State::Hurt || state == State::ParriedRight || state == State::ParriedLeft);
	}
	virtual CollisionBox getParryBox() {
		return spearBox;
	}
	virtual void getParried(Vector2f dirVec) {
		std::printf("getParried\n");

		if (semiCross(dirVec, spearDirVec) < 0) {
			setState(State::ParriedRight);
		} else {
			setState(State::ParriedLeft);
		}
	}

	virtual bool canHurt() {
		return (state == State::Point);
	}
	virtual bool canGetHurt() {
		return (state != State::Hurt);
	}
};

#endif
