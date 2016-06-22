
#include "Player.hpp"

#include "Resources.hpp"

#include <iostream>

using namespace std;


#define MOVEMENT_SPEED (700.0f)


Player::Player(Vector2f position, Controls *con):
	Object(position),
	Controlled(con)
{

	texture.loadFromFile("media/images/char.png");
	sprite.setTexture(texture);
	sprite.setOrigin(sf::Vector2f((float)texture.getSize().x / 2.0f, (float)texture.getSize().y));
}

void Player::eventCallback(int id) {
	switch (id) {
		case 0: {
			printf("bam!\n");
		} break;
		default: cout << "Action button pressed: " << id << endl;
	}
}

void Player::update(float elapsedTime) {

	Vector2f v = controls->movement();

	pos += (v * MOVEMENT_SPEED) * elapsedTime;

}

void Player::draw(RenderTarget *window) {
	sprite.setPosition(pos);
	window->draw(sprite);
}
