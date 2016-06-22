#ifndef _GAME_H_
#define _GAME_H_

#include "Object.hpp"
#include "Player.hpp"
#include "obj/Orb.hpp"

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
    sf::RenderTexture monitorPixelArea;

	sf::Texture mapTex;
	sf::Sprite mapSprite;

	std::vector<Object *> *objects;
	std::vector<Orb *> *orbs;
	std::vector<Player *> *players;
    
    float scaleFactor;
    
	float orbTimer = 0;
};

#endif
