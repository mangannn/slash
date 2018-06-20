#ifndef _Animation_H_
#define _Animation_H_

#include "Functions.hpp"
#include "Resources.hpp"


class Animation {

public:

	Vector2f *offset;

	Vector2f pos;

	int num;

	sf::Texture **ani;
	sf::Sprite sprite;

	int current;

	Animation(Vector2f *offsetParam, Vector2f position, std::string filename, int numParam):
		offset(offsetParam),
		pos(position),
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
		sprite.setOrigin(sf::Vector2f((float)ani[current]->getSize().x / 2.f, (float)ani[current]->getSize().y /2.f));
	}

	virtual void draw(RenderTarget *target) {
		sprite.setPosition(getPosition());
		target->draw(sprite);
	}

	Vector2f getPosition() {
		return (*offset) + pos;
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
