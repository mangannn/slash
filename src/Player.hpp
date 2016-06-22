#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Object.hpp"
#include "Collision.hpp"

using namespace sf;

Color tintColor(Color c, int amount);

class Player: public Object {

public:

	Texture *texture;
	Sprite sprite;
	int joystickId;


	Vector2f swordDir;
	float swordLen;

	CollisionBox walkBox, bodyBox, swordBox;

	Player(Vector2f position, int joystickIdParam);

	virtual ~Player() {}
	
	virtual void update(float elapsedTime);

	virtual void draw(RenderTarget *window, RenderTarget *monitor);
    
private:
    
    Color swordColor = Color(220, 30, 40);
    const static int SWORD_VERTEX_COUNT = 6;
    Vertex swordVertices[SWORD_VERTEX_COUNT] = {
        Vertex(Vector2f(0, 0), swordColor),
        Vertex(Vector2f(0, 0), tintColor(swordColor, 1)),
        Vertex(Vector2f(0, 0), tintColor(swordColor, 2)),
        Vertex(Vector2f(0, 0), tintColor(swordColor, 3)),
        Vertex(Vector2f(0, 0), tintColor(swordColor, 4)),
        Vertex(Vector2f(0, 0), tintColor(swordColor, 5))
    };
};

#endif
