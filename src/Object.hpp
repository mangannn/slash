#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <SFML/Graphics.hpp>

using namespace sf;

class Object {

public:

	Vector2f pos;

	Object(
		Vector2f posParam
		):
		pos(posParam)
	{}
	virtual ~Object() {}

	virtual void update(float elapsedTime) {}

	virtual void draw(RenderTarget *window) {}
};

#endif