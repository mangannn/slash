#ifndef _Player_
#define _Player_

#include "../obj/Character.hpp"
#include "../Collision.hpp"
#include "../Functions.hpp"

#include "../Animation.hpp"

#include "Weapon.hpp"

#include "Controls.hpp"

using namespace sf;

class Player: public Character, public Controlled {

public:

	float direction = 0;
	float velocity = 0;
    Vector2f posStart;

	Sprite sprite;
	Animation ani, feetAnimation;

	float feetTimer = 0;
	int feetPos = 0;

	Player(Vector2f position, Controls *con);

	virtual ~Player();
	
	virtual void eventCallback(int id);
	
	virtual void update(float elapsedTime);

	virtual void draw(RenderTarget *window);

	virtual void hit(Vector2f direction);
};

#endif
