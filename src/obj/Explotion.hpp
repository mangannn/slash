#ifndef _Explotion_H_
#define _Explotion_H_

#include "../Object.hpp"
#include "../Functions.hpp"
#include "../Resources.hpp"
#include "Animation.hpp"


class Explotion: public Object {

public:

	Animation ani;

	float timer = 0;

	Explotion(Vector2f position):
		Object(position),
		ani(&pos, "media/images/poff", 9)
	{
	}

	virtual void update(float elapsedTime) {
		timer += elapsedTime;

		if (timer > 0.08) {
			timer -= 0.08;
			ani.setFrame(ani.getFrame() + 1);
		}
	}

	virtual void draw(RenderTarget *target, RenderTarget *monitor) {

		ani.draw(target);
	}
};

#endif
