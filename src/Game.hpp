#ifndef _GAME_H_
#define _GAME_H_

class Map;
class Player;
class Object;

class Game {

public:

	Game();
	virtual ~Game();


	virtual void eventHandle(sf::Event event);
	virtual void update(float elapsedTime);

	virtual void draw(sf::RenderTarget *target);


	/*static void add(Object *o) {
		objects->push_back(o);
	}*/

private:

	sf::View gameView;
    sf::RenderTexture gamePixelArea;
    sf::RenderTexture monitorPixelArea;


    float scaleFactor;

	Map *map;

};

#endif
