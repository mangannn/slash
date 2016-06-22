#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Object.hpp"
#include "Collision.hpp"

class Player: public Object {

public:

	sf::Texture *texture;
	sf::Sprite sprite;


	sf::Vector2f swordDir;
	float swordLen;

	CollisionBox walkBox, swordBox;

	Player(Vector2f position);

	virtual ~Player() {}
	
	virtual void update(float elapsedTime);

	virtual void draw(RenderTarget *window);
};

#endif
