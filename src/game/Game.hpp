#ifndef _Game_
#define _Game_

class Map;

class Game {

public:

	Game();
	virtual ~Game();


	virtual void eventHandle(sf::Event event);
	virtual void update(float elapsedTime);

	virtual void draw(sf::RenderTarget *target);

private:

	sf::View gameView;

	float scaleFactor;

	Map *map;

};

#endif
