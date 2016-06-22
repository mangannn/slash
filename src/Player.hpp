#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Object.hpp"
#include "Collision.hpp"

using namespace sf;

Color tintColor(Color c, int amount) {
    c.r += 10 * amount;
    c.g += 5 *amount;
    c.b += -30 * amount;
    return c;
}

class Player: public Object {

public:

	Texture *texture;
	Sprite sprite;


	Vector2f swordDir;
	float swordLen;

	CollisionBox walkBox, swordBox;

	Player(Vector2f position);

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
