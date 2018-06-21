#include "Spawn.hpp"

#include "enemies/Frog.hpp"
#include "enemies/Imp.hpp"
#include "projectiles/Orb.hpp"

#include "../World.hpp"

Spawn::Spawn(Vector2f position, std::string typeParam):
	Object(position),
	type(typeParam)
{
	std::string name = type.substr(0, type.find(":"));

	if (name == "frog") {
		what = 0;
	} else if (name == "orb") {
		what = 1;
	} else {
		what = 3;
	}

	spawnAction();
}

void Spawn::update(float elapsedTime) {
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

void Spawn::spawnAction() {

	std::string name = type.substr(0, type.find(":"));
	std::string arg1 = type.substr(type.find(":") + 1);

	if (name == "player") {

		int actionButton[3] = {sf::Keyboard::Z, sf::Keyboard::X, sf::Keyboard::Space};
		Controls *con = new KeyboardControls(sf::Keyboard::Up, sf::Keyboard::Down, sf::Keyboard::Left, sf::Keyboard::Right, actionButton, 2);
		World::addPlayer(new Player(pos, con));

		World::removeSpawn(this);
	} else if (name == "playerdummy") {

		int t = atoi(arg1.c_str());
		Controls *con = new DummyControls(t);
		World::addPlayer(new Player(pos, con));
		
		World::removeSpawn(this);
	} else if (name == "imp") {

		int t = atoi(arg1.c_str());
		World::add(new Imp(pos, t));

		World::removeSpawn(this);
	}
}

#ifdef DEBUG
void Spawn::drawDebug(RenderTarget *target) {

	const float r = 20.f;

	sf::CircleShape shape(r);
	shape.setFillColor(sf::Color(0, 0, 0, 0));
	shape.setOutlineThickness(1);
	shape.setOutlineColor(sf::Color(0, 250, 50));
	shape.setPosition(pos - Vector2f(r, r));
	target->draw(shape);
}
#endif
