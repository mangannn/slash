#ifndef _Image_H_
#define _Image_H_

#include "../Object.hpp"
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

		sf::Texture *texture = getTexture(filename);
		sprite.setTexture(*texture);
		sprite.setOrigin(sf::Vector2f((float)texture->getSize().x / 2.0f, (float)texture->getSize().y));
	}

	virtual ~Image() {}

	virtual void draw(RenderTarget *target) {
		sprite.setPosition(pos);
		target->draw(sprite);
	}
};

}

#endif
