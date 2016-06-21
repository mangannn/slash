#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Object.hpp"
#include "Controls.hpp"


class Player: public Object, public Controlled {

public:

	sf::Texture texture;
	sf::Sprite sprite;

	Player(Vector2f position, Controls *controls);

	virtual ~Player() {}

	virtual void eventCallback(int id);

	virtual void update(float elapsedTime);

	virtual void draw(RenderWindow *window);
};

#endif
