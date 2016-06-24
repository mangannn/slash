#ifndef _Spawn_H_
#define _Spawn_H_

#include "../Object.hpp"
#include "../Functions.hpp"

class Spawn: public Object {

public:

	std::string type;

	float r;

	Spawn():
		Object(Vector2f(0,0))
	{}
	Spawn(Vector2f position, std::string typeParam, float radius):
		Object(position),
		type(typeParam),
		r(radius)
	{
	}

	virtual void update(float elapsedTime) {

	}

	virtual void draw(RenderTarget *target) {

		// for debug
		sf::CircleShape shape(r);
		shape.setFillColor(sf::Color(0, 0, 0, 0));
		shape.setOutlineThickness(1);
		shape.setOutlineColor(sf::Color(0, 250, 50));
		shape.setPosition(pos - Vector2f(r, r));
		target->draw(shape);
	}
};

#endif
