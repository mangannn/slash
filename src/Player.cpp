
#include "Player.hpp"

#include "Resources.hpp"

#include <iostream>

using namespace std;


#define MOVEMENT_SPEED (700.0f)


Player::Player(Vector2f position, Controls *con):
	Object(position),
	Controlled(con)
{

	sprite.setTextureRect(sf::IntRect(0, 0, playerSpriteSize.x, playerSpriteSize.y));
	sprite.setTexture(standingTex);
	sprite.setOrigin(sf::Vector2f((float)playerSpriteSize.x / 2.0f, (float)playerSpriteSize.y / 2.0f) + Vector2f(0, -30));

	//sprite.setColor(color);

	float radius = 1;

	Vector2f scale = sf::Vector2f((radius * 2.0f) / (float)playerSpriteSize.x, (radius * 2.0f) / (float)playerSpriteSize.x);

	sprite.setScale(scale * 1.1f);
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

void Player::draw(RenderWindow *window) {
	sprite.setRotation(90);
	sprite.setPosition(pos);
	window->draw(sprite);
}
