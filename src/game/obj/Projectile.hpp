#ifndef _Projectile_H_
#define _Projectile_H_

#include "../Object.hpp"
#include "../Functions.hpp"
#include "../Resources.hpp"


class Projectile: public Object {

public:

	sf::Sprite sprite;

	Vector2f vel;

	CollisionBox box;

	Projectile(Vector2f position, Vector2f velocity, std::string imageFile):
		Object(position),
		vel(velocity)
	{

		sf::Texture *texture = Resources::getTexture(imageFile);
		sprite.setTexture(*texture);
		sprite.setOrigin(sf::Vector2f((float)texture->getSize().x / 2.0f, (float)texture->getSize().y));

		sprite.setColor(RANDOM_COLOR);

		box = CollisionBox(&pos, Vector2f(0,-(float)texture->getSize().y / 2.0f), (float)texture->getSize().x / 2.0f);
	}

	virtual ~Projectile() {}

	virtual void update(float elapsedTime) {
		pos += vel * elapsedTime;
	}

	virtual void draw(RenderTarget *target, RenderTarget *monitor) {

		sprite.setPosition(pos);
		target->draw(sprite);

		//box.draw(target);
	}
};

#endif
