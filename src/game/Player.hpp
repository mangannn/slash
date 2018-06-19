#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Object.hpp"
#include "Collision.hpp"
#include "Functions.hpp"

#include "obj/Animation.hpp"

#include "parts_player/SwordGraphics.hpp"

using namespace sf;

class Player: public Object {

public:


	Vector2f vel;

	Texture *texture;
	Sprite sprite;
	Animation ani;
	float legsTimer;


	int joystickId;


	CollisionBox walkBox, bodyBox, swordBox;

	Vector2f swordTip, swordOri;
	float swordLen;
	SwordGraphics swordGraphics;

	Player(Vector2f position, int joystickIdParam);

	virtual ~Player() {}
	
	virtual void update(float elapsedTime);

	virtual void draw(RenderTarget *window, RenderTarget *monitor);
	
};

#endif
