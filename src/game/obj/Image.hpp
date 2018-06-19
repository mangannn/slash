#ifndef _Image_
#define _Image_

#include "Object.hpp"
#include "../Functions.hpp"
#include "../Resources.hpp"


namespace cs {

class Image: public Object {

public:

	sf::Sprite sprite;

	Image():
		Object(Vector2f(0,0))
	{}
	Image(Vector2f position, std::string filename):
		Object(position)
	{

		sf::Texture *texture = Resources::getTexture(filename);
		sprite.setTexture(*texture);
		sprite.setOrigin(sf::Vector2f((float)texture->getSize().x / 2.0f, (float)texture->getSize().y));
		int a = 70;
		sprite.setColor(Color(255 - a * RANDOM, 255 - a * RANDOM, 255 - a * RANDOM));
	}

	virtual ~Image() {}

	virtual void draw(RenderTarget *target) {
		sprite.setPosition(pos);
		target->draw(sprite);
	}
};

}

#endif
