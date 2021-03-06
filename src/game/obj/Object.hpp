#ifndef _Object_
#define _Object_

#include <SFML/Graphics.hpp>

using namespace sf;

class Object {

public:

	Vector2f pos;

	Object(Vector2f posParam):
		pos(posParam)
	{}
	virtual ~Object() {}

	virtual void update(float elapsedTime) {}

	virtual void draw(RenderTarget *target) {}

#ifdef DEBUG
	virtual void drawDebug(RenderTarget *target) {}
#endif

};

#endif