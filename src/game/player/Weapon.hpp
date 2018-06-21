#ifndef _Weapon_
#define _Weapon_

#include <SFML/Graphics.hpp>
#include "../Collision.hpp"

using namespace sf;

class Weapon {

public:

	Vector2f *offset;

	enum class Action {
		Slash,
		Stab,
		GetHit
	};

	Weapon(Vector2f *offsetParam):
		offset(offsetParam)
	{

	}

	virtual ~Weapon() {}
	
	virtual void update(float elapsedTime, float characterDirection) = 0;

	virtual void draw(RenderTarget *target) = 0;

#ifdef DEBUG
	virtual void drawDebug(RenderTarget *target) {}
#endif

	virtual bool isBehind() = 0;

	virtual void setAction(Action a) = 0;

	virtual bool isHitCircle(CollisionBox c, Vector2f *returnDirVec) = 0;

	virtual bool isBusy() {
		return false;
	}

	virtual float getCharacterVelocity() = 0;

	virtual bool canParry() {return false;}
	virtual bool canBeParried() {return false;}
	virtual CollisionBox getParryBox() = 0;
	virtual void getParried(Vector2f dirVec) = 0;

	virtual bool canHurt() {return true;}
	virtual bool canGetHurt() {return true;}
};

#endif
