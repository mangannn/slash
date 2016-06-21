#ifndef _ORB_H_
#define _ORB_H_

#include "../Object.hpp"

class Orb: public Object {

public:

	sf::Sprite sprite;
	sf::Texture texture;

	Vector2f vel;

	float radius;

	Orb(Vector2f position, Vector2f velocity):
		Object(position),
		vel(velocity)
	{

		texture.loadFromFile("media/images/orb.png");
		sprite.setTexture(texture);
		sprite.setOrigin(sf::Vector2f((float)texture.getSize().x / 2.0f, (float)texture.getSize().y / 2.0f));

		radius = (float)texture.getSize().x / 2.0f;
	}

	virtual ~Orb() {}

	virtual void update(float elapsedTime) {
		pos += vel * elapsedTime;
	}

	virtual void draw(RenderWindow *window) {
		sprite.setPosition(pos);
		window->draw(sprite);
	}
};

#endif
