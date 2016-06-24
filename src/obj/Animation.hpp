#ifndef _Animation_H_
#define _Animation_H_

#include "../Object.hpp"
#include "../Functions.hpp"
#include "../Resources.hpp"


class Animation {

public:

	Vector2f *offset;

	int num;

	sf::Texture **ani;
	sf::Sprite sprite;

	int current;

	Animation(Vector2f *offsetParam, std::string filename, int numParam):
		offset(offsetParam),
		num(numParam),
		current(0)
	{

		ani = new sf::Texture*[num]();

		for (int i = 0; i < num; i++) {
			ani[i] = getTexture(filename + "000" + std::to_string(i) + ".png");
		}
		sprite.setTexture(*ani[current]);
		sprite.setOrigin(sf::Vector2f((float)ani[current]->getSize().x / 2.0f, (float)ani[current]->getSize().y));
	}

	virtual void draw(RenderTarget *target) {
		sprite.setPosition(*offset);
		target->draw(sprite);
	}

	void setFrame(int i) {
		if (i < num) {
			current = i;
			sprite.setTexture(*ani[current]);
		}
	}
	int getFrame() {
		return current;
	}
};

#endif
