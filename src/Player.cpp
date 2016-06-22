
#include "Player.hpp"

#include "Resources.hpp"
#include "Functions.hpp"

#include <iostream>

using namespace std;


#define MOVEMENT_SPEED (700.0f)

#define JOYSTICK_THRESHOLD (0.2f)


Player::Player(Vector2f position, int joystickIdParam):
	Object(position),
	joystickId(joystickIdParam),
	walkBox(&pos, Vector2f(0, -14), 14),
	bodyBox(&pos, Vector2f(0, -25), 20),
	swordBox(&pos, Vector2f(0, 10), 4)
{
	texture = getTexture("media/images/char.png");
	sprite.setTexture(*texture);
	sprite.setOrigin(sf::Vector2f((float)texture->getSize().x / 2.0f, (float)texture->getSize().y));

	sprite.setColor(RANDOM_COLOR);
}

void Player::update(float elapsedTime) {

	float x = sf::Joystick::getAxisPosition(joystickId, sf::Joystick::X) / 100.0f;
	float y = sf::Joystick::getAxisPosition(joystickId, sf::Joystick::Y) / 100.0f;

	Vector2f v = Vector2f(x, y);

	x = sf::Joystick::getAxisPosition(joystickId, sf::Joystick::U) / 100.0f;
	y = sf::Joystick::getAxisPosition(joystickId, sf::Joystick::V) / 100.0f;

	Vector2f v2 = Vector2f(x, y);

	float sv2 = size(v2);
	swordLen = 40;

	if (sv2 > JOYSTICK_THRESHOLD) {
		swordLen *= sv2;
		swordDir = v2 / sv2;
	}


	Vector2f swordOri = Vector2f(0, -(float)texture->getSize().y / 2.0);
	swordBox.pos = swordOri + swordDir * swordLen;

	if (size(v) > JOYSTICK_THRESHOLD) {
		pos += (v * MOVEMENT_SPEED) * elapsedTime;
	}

}

void Player::draw(RenderTarget *target, RenderTarget *monitor) {

	sprite.setPosition(pos);
	target->draw(sprite);

	walkBox.draw(target);
	bodyBox.draw(target);
	swordBox.draw(target);


	Vector2f swordOri = pos + Vector2f(0, -(float)texture->getSize().y / 2.0);

	sf::Vertex line[] =
	{
	    swordOri,
	    swordOri + swordDir * swordLen
	};

	target->draw(line, 2, sf::Lines);
}
