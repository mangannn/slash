
#include "Player.hpp"

#include "Resources.hpp"
#include "Functions.hpp"

#include <iostream>
#include <GL/gl.h>


using namespace std;


#define MOVEMENT_SPEED (400.0f)

#define JOYSTICK_THRESHOLD (0.2f)


Player::Player(Vector2f position, int joystickIdParam):
	Object(position),
	ani(&pos, Vector2f(0, 3), "media/ani/feet", 6),
	legsTimer(0),
	joystickId(joystickIdParam),
	walkBox(&pos, Vector2f(0, -14), 14),
	bodyBox(&pos, Vector2f(0, -25), 20),
	swordBox(&pos, Vector2f(0, 10), 4)
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
	
	float jStickSwordAmount = size(jStickSwordPos);

	if (jStickSwordAmount > JOYSTICK_THRESHOLD) {
		swordLen = jStickSwordAmount * 40;
		swordDir = jStickSwordPos / jStickSwordAmount;
	}
	else {
		swordLen = 0;
	}
	


	Vector2f swordOri = Vector2f(0, -(float)texture->getSize().y / 2.0);
	swordBox.pos = swordOri + swordDir * swordLen;


	float jStickMovementAmount = size(jStickMovementPos);


	float mov = 0;

	if (jStickMovementAmount > JOYSTICK_THRESHOLD) {
		pos += (jStickMovementPos * MOVEMENT_SPEED) * elapsedTime;
		mov = jStickMovementAmount * MOVEMENT_SPEED;
	}

	legsTimer += 0.08 * mov * elapsedTime;
	if (legsTimer > 1.0f) {
		legsTimer -= 1.0f;

		if (!ani.setFrame(ani.getFrame() + 1)) {
			ani.setFrame(0);
		}
	} 
	
	updateSwordGraphics(elapsedTime);

}

void Player::draw(RenderTarget *target, RenderTarget *monitor) {

	ani.draw(target);

	sprite.setPosition(pos);
	target->draw(sprite);


	//walkBox.draw(target);
	//bodyBox.draw(target);
	//swordBox.draw(target);


	//Vector2f swordOri = pos + Vector2f(0, -(float)texture->getSize().y / 2.0);

	
	
	Vertex line[] =
	{
		Vertex(swordVertices[1].position, Color::White),
		Vertex(swordDir * swordLen, Color::White)
	};
	

	Transform t;
	t.translate(pos + Vector2f(0, -(float)texture->getSize().y / 2.0));
	
	target->draw(line, 2, Lines, t);
	target->draw(swordVertices, SWORD_VERTEX_COUNT, TrianglesFan, t);
	
	
}


void Player::updateSwordGraphics(float elapsedTime) {
	
	approach(&swordVertices[1].position, swordDir * swordLen, 0.8);
	// Follow sword edge
	for (int i = 2; i < SWORD_VERTEX_COUNT; i++) {
		approach(&swordVertices[i].position, swordVertices[i - 1].position, 0.2);
	}
	
}
