#ifndef _GAME_H_
#define _GAME_H_

#include "Object.hpp"
#include "Player.hpp"
#include "obj/Orb.hpp"

#include "Map.hpp"

class Game {

public:

	Game();
	virtual ~Game();


	virtual void eventHandle(sf::Event event);
	virtual void update(float elapsedTime);

	virtual void draw(sf::RenderTarget *window);

private:

	sf::View gameView;
    sf::RenderTexture gamePixelArea;


	std::vector<Object *> *objects;
	std::vector<Orb *> *orbs;
	std::vector<Player *> *players;

	Map *map;


	float orbTimer = 0;
};

#endif
