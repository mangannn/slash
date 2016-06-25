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

		char str[20];

		for (int i = 0; i < num; i++) {
			sprintf(str, "%04d", i);
			ani[i] = Resources::getTexture(filename + str + ".png");
		}
		sprite.setTexture(*ani[current]);
		sprite.setOrigin(sf::Vector2f((float)ani[current]->getSize().x / 2.0f, (float)ani[current]->getSize().y));
	}

	virtual void draw(RenderTarget *target) {
		sprite.setPosition(*offset);
		target->draw(sprite);
	}

	bool setFrame(int i) {
		if (i < num) {
			current = i;
			sprite.setTexture(*ani[current]);
			return true;
		}
		return false;
	}
	int getFrame() {
		return current;
	}
};

#endif
