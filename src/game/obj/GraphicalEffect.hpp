#ifndef _GraphicalEffect_
#define _GraphicalEffect_

#include "Object.hpp"
#include "../Animation.hpp"

#include "../World.hpp"


class GraphicalEffect: public Object {

public:

	Animation ani;

	float timer;
	float speed;

	GraphicalEffect(Vector2f position, std::string imageFile, int num, float speedParam):
		Object(position),
		ani(&pos, Vector2f(0,0), imageFile, num),
		timer(0),
		speed(speedParam)
	{
	}

	virtual void update(float elapsedTime) {
		timer += elapsedTime;

		if (timer > speed) {
			timer -= speed;

			if (!ani.setFrame(ani.getFrame() + 1)) {
				World::remove(this);
			}
		}
	}

	virtual void draw(RenderTarget *target) {
		ani.draw(target);
	}
};

#endif
