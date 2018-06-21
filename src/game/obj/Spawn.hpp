#ifndef _Spawn_
#define _Spawn_

#include "Object.hpp"
#include "../Functions.hpp"

#include "enemies/Frog.hpp"
#include "projectiles/Orb.hpp"

#include "../World.hpp"



class Spawn: public Object {

public:

	std::string type;

	float r;

	float timer = 0;
	float spawnIntervall = 4.9;


	float what;

	Spawn():
		Object(Vector2f(0,0))
	{}
	Spawn(Vector2f position, std::string typeParam, float radius):
		Object(position),
		type(typeParam),
		r(radius)
	{
		std::string name = type.substr(0, type.find(":"));

		if (name == "frog") {
			what = 0;
		} else if (name == "orb") {
			what = 1;
		} else if (name == "player") {
			what = 2;
		} else {
			what = 3;
		}
	}

	virtual void update(float elapsedTime) {
		timer += elapsedTime;

		if (timer > spawnIntervall) {
			timer -= spawnIntervall;

			if (what == 0) {
				World::add(new Frog(pos));
			} else if (what == 1) {
				World::add(new Orb(pos, 40.0f * Vector2f(RANDOM2, RANDOM2)));
			}
		}
	}

#ifdef DEBUG
	virtual void drawDebug(RenderTarget *target) {
		sf::CircleShape shape(r);
		shape.setFillColor(sf::Color(0, 0, 0, 0));
		shape.setOutlineThickness(1);
		shape.setOutlineColor(sf::Color(0, 250, 50));
		shape.setPosition(pos - Vector2f(r, r));
		target->draw(shape);
	}
#endif

};

#endif
