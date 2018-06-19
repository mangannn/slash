#ifndef _COLL_H_
#define _COLL_H_

#include "Object.hpp"
#include "Functions.hpp"


class CollisionBox {

public:

	Vector2f *offset;

	Vector2f pos;
	float r;

	CollisionBox() {}
	CollisionBox(Vector2f *offsetParam, Vector2f position, float radius):
		offset(offsetParam),
		pos(position),
		r(radius)
	{
	}

	static bool check(CollisionBox a, CollisionBox b) {
		float coll_dist = a.r + b.r;
		Vector2f diff = a.getPosition() - b.getPosition();

		if (sqrSize(diff) < coll_dist * coll_dist) {
			return true;
		}
		return false;
	}

	void draw(RenderTarget *target) {

		// for debug
		sf::CircleShape shape(r);
		shape.setFillColor(sf::Color(0, 0, 0, 0));
		shape.setOutlineThickness(1);
		shape.setOutlineColor(sf::Color(250, 150, 100));
		shape.setPosition(getPosition() - Vector2f(r, r));
		target->draw(shape);
	}

	Vector2f getPosition() {
		return (*offset) + pos;
	}
};

#endif