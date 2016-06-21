#ifndef _GAME_H_
#define _GAME_H_

#include "Object.hpp"
#include "Player.hpp"

class Game {

public:

	Game();
	virtual ~Game();


	virtual void eventHandle(sf::Event event);
	virtual void update(float elapsedTime);

	virtual void draw(sf::RenderWindow *window);

private:

	sf::View gameView;

	std::vector<Object *> *objects;
	std::vector<Player *> *players;
};

#endif