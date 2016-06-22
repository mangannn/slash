
#include "Player.hpp"

#include "Resources.hpp"
#include "Functions.hpp"

#include <iostream>
#include <GL/gl.h>


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
	sprite.setOrigin(Vector2f((float)texture->getSize().x / 2.0f, (float)texture->getSize().y));

	sprite.setColor(RANDOM_COLOR);
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

	swordLen = 40;
	
    float jStickSwordAmount = size(jStickSwordPos);

	if (jStickSwordAmount > JOYSTICK_THRESHOLD) {
		swordLen *= jStickSwordAmount;
		swordDir = jStickSwordPos / jStickSwordAmount;
	}


	Vector2f swordOri = Vector2f(0, -(float)texture->getSize().y / 2.0);
	swordBox.pos = swordOri + swordDir * swordLen;

	if (size(jStickMovementPos) > JOYSTICK_THRESHOLD) {
		pos += (jStickMovementPos * MOVEMENT_SPEED) * elapsedTime;
	}

}

void Player::draw(RenderTarget *target, RenderTarget *monitor) {

	sprite.setPosition(pos);
	target->draw(sprite);

	walkBox.draw(target);
	bodyBox.draw(target);
	swordBox.draw(target);


	//Vector2f swordOri = pos + Vector2f(0, -(float)texture->getSize().y / 2.0);

    
    
	Vertex line[] =
	{
        Vertex(swordVertices[1].position, Color::White),
        Vertex(swordDir * swordLen, Color::White)
	};
    
    swordVertices[2].position = swordVertices[1].position;
    swordVertices[1].position = swordDir * swordLen;
    

    Transform t;
    t.translate(pos + Vector2f(0, -(float)texture->getSize().y / 2.0));
    
	target->draw(line, 2, Lines, t);
    target->draw(swordVertices, SWORD_VERTEX_COUNT, TrianglesFan, t);
    
}



Color tintColor(Color c, int amount) {
    c.r += 10 * amount;
    c.g += 5 *amount;
    c.b += -30 * amount;
    return c;
}
