
#include <iostream>
#include "Player.hpp"

#include "Sword.hpp"
#include "Spear.hpp"

#include "../Resources.hpp"
#include "../Functions.hpp"
#include "../obj/graphicaleffects/Explotion.hpp"

#include <math.h>

Player::Player(Vector2f position, Controls *con):
	Character(position, 20),
	Controlled(con),

	ani(&pos, Vector2f(0, 0), "media/animation/Billy/body", 8),
	feetAnimation(&pos, Vector2f(0, 0), "media/animation/Billy/feet", 24)

{

	walkBox = CollisionBox(&pos, Vector2f(0, 18), 8);
	bodyBox = CollisionBox(&pos, Vector2f(0, 7), 12);

	weapon = new Sword(&pos);
}

Player::~Player() {
	delete weapon;
}


void Player::eventCallback(int id) {
	switch (id) {
		case 0: {
			weapon->setAction(Weapon::Action::Slash);
		} break;
		case 1: {
			weapon->setAction(Weapon::Action::Stab);
		} break;
		default: std::cout << "Action button pressed: " << id << "\n";
	}
}

void Player::update(float elapsedTime) {


	const float ACCELRATION_SPEED = 1200.f;
	const float MOVEMENT_SPEED = 300.f;

	const float TURN_SPEED = 14.f;

	const float MOVE_THRESHOLD = 0.2f;


	Vector2f movementVec = controls->movement();
	float movementAmount = size(movementVec);

	Vector2f directionVec;


	if (weapon->isBusy()) {
		direction = angleFromDiscreteDirection(discreteDirectionFromAngle(direction, 8), 8);
		velocity = weapon->getCharacterVelocity();

	} else if (movementAmount > MOVE_THRESHOLD) {

		float movementDir = atan2(movementVec.y, movementVec.x);

		direction = movementDir;//towardsAngle(direction, movementDir, TURN_SPEED * elapsedTime);

		velocity = increse(velocity, ACCELRATION_SPEED * elapsedTime, MOVEMENT_SPEED);
	} else {
		velocity = decrese(velocity, elapsedTime * ACCELRATION_SPEED);
	}


	int dir = discreteDirectionFromAngle(direction, 8);
	/*if (weapon.state == Sword::State::Swirl) {
		dir = discreteDirectionFromAngle(direction + weapon.currentSwordMovement.getDirection(), 8);
	}*/
	ani.setFrame(dir);

	feetAnimation.setFrame(6 * discreteDirectionFromAngle(direction, 4) + feetPos);

	feetTimer += elapsedTime * velocity * 0.1f;
	if (feetTimer > 1.f) {
		feetTimer -= 1.f;
		feetPos++;
		if (feetPos >= 6) {
			feetPos = 0;
		}
	}


	pos += Vector2f(cos(direction), sin(direction)) * (velocity * elapsedTime);

	weapon->update(elapsedTime, direction);
}

void Player::draw(RenderTarget *target) {

	float r = 15;
	sf::CircleShape shape(r);
	shape.setFillColor(sf::Color::Red);
	shape.setOrigin(r,3*r);
	shape.setPosition(pos);


	if (weapon->isBehind()) {
		weapon->draw(target);
		ani.draw(target);
		feetAnimation.draw(target);
		//target->draw(shape);
	} else {
		ani.draw(target);
		feetAnimation.draw(target);
		//target->draw(shape);
		weapon->draw(target);
	}

	walkBox.draw(target);
	bodyBox.draw(target);
}


void Player::hit(Vector2f direction) {

	std::cout << "player hurt\n";

	hp--;
	weapon->setAction(Weapon::Action::GetHit);

	if (hp <= 0) {
		World::add(new Explotion(pos));
		std::cout << "Dead\n";
		hp = 20;
	}
}