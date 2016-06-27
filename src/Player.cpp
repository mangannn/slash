
#include <iostream>
#include "Player.hpp"

#include "Resources.hpp"
#include "Functions.hpp"

#include <math.h>


using namespace std;


#define MOVEMENT_SPEED (400.0f)

#define JOYSTICK_THRESHOLD (0.2f)

#define SWORD_LENGTH (40.f)
#define SWORD_LENGTH_MAX (40.f)
#define SWORD_LENGTH_MIN (20.f)


Player::Player(Vector2f position, int joystickIdParam):
	Object(position),
	ani(&pos, Vector2f(0, 3), "media/ani/feet", 6),
	legsTimer(0),
	joystickId(joystickIdParam),
	walkBox(&pos, Vector2f(0, -14), 14),
	bodyBox(&pos, Vector2f(0, -25), 20),

	swordBox(&pos, Vector2f(0, 10), 4),
	swordTip(0, SWORD_LENGTH)
{
	texture = Resources::getTexture("media/images/soul.png");
	sprite.setTexture(*texture);
	sprite.setOrigin(Vector2f((float)texture->getSize().x / 2.0f, (float)texture->getSize().y));

	//sprite.setColor(RANDOM_COLOR);
}

void Player::update(float elapsedTime) {
	
	Vector2f jStickMovementPos = Vector2f(
		Joystick::getAxisPosition(joystickId, Joystick::X) / 100.0f,
		Joystick::getAxisPosition(joystickId, Joystick::Y) / 100.0f
	);
	
	Vector2f jStickSwordPos = Vector2f(
		Joystick::getAxisPosition(joystickId, Joystick::U) / 100.0f,
		Joystick::getAxisPosition(joystickId, Joystick::V) / 100.0f
	);

	
	float jStickMovementAmount = size(jStickMovementPos);
	float jStickSwordAmount = size(jStickSwordPos);




	if (jStickMovementAmount > JOYSTICK_THRESHOLD) {
		pos += jStickMovementPos * (MOVEMENT_SPEED * elapsedTime);
	}



	legsTimer += 0.08 * (MOVEMENT_SPEED * jStickMovementAmount) * elapsedTime;
	if (legsTimer > 1.0f) {
		legsTimer -= 1.0f;

		if (!ani.setFrame(ani.getFrame() + 1)) {
			ani.setFrame(0);
		}
	}




	if (jStickSwordAmount > JOYSTICK_THRESHOLD) {

		swordTip = approach(swordTip, jStickSwordPos * (SWORD_LENGTH / jStickSwordAmount), elapsedTime * 15.0f);
	} else if (jStickMovementAmount > JOYSTICK_THRESHOLD) {
		float dir = atan2(jStickMovementPos.y, jStickMovementPos.x) + M_PI * (5.0f / 6.0f);
		swordTip = approach(swordTip, Vector2f(cos(dir), sin(dir)) * SWORD_LENGTH_MIN, elapsedTime * 30.0f); 
	}
	


	swordOri = Vector2f(0, -(float)texture->getSize().y / 2.0) + (swordTip / size(swordTip)) * 10.0f;
	swordBox.pos = Vector2f(0, -(float)texture->getSize().y / 2.0) + swordTip;


	
	swordGraphics.update(elapsedTime, swordTip);

}

void Player::draw(RenderTarget *target, RenderTarget *monitor) {

	ani.draw(target);

	sprite.setPosition(pos);



	if (swordTip.y >= 0) {
		target->draw(sprite);
		swordGraphics.draw(target, pos + Vector2f(0, -(float)texture->getSize().y / 2.0), swordTip);
	} else {
		swordGraphics.draw(target, pos + Vector2f(0, -(float)texture->getSize().y / 2.0), swordTip);
		target->draw(sprite);
	}


	//walkBox.draw(target);
	//bodyBox.draw(target);
	//swordBox.draw(target);	
}