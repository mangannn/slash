#ifndef _ROCK_H_
#define _ROCK_H_

#include "../Object.hpp"

#include "../Resources.hpp"


class Rock: public Object {

public:

	sf::Sprite sprite;

	Rock(Vector2f position):
		Object(position)
	{
		sprite.setTextureRect(sf::IntRect(0, 0, playerSpriteSize.x, playerSpriteSize.y));
		sprite.setTexture(standingTex);
		sprite.setOrigin(sf::Vector2f((float)playerSpriteSize.x / 2.0f, (float)playerSpriteSize.y / 2.0f) + Vector2f(0, -30));

		sprite.setColor(Color(0,0,255));

		float radius = 1;

		Vector2f scale = sf::Vector2f((radius * 2.0f) / (float)playerSpriteSize.x, (radius * 2.0f) / (float)playerSpriteSize.x);

		sprite.setScale(scale * 1.1f);
	}

	virtual ~Rock() {}

	virtual void update(float elapsedTime) {}

	virtual void draw(RenderTarget *window) {
		sprite.setPosition(pos);
		window->draw(sprite);
	}
};

#endif
