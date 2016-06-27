#ifndef _SWORD_GRAPHICS_H_
#define _SWORD_GRAPHICS_H_

#include <SFML/Graphics.hpp>

class SwordGraphics {

public:
	
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
	Vector2f swordVelocities[SWORD_VERTEX_COUNT] = { // (rotspeed:growspeed)
		Vector2f(0, 0),
		Vector2f(0, 0),
		Vector2f(0, 0),
		Vector2f(0, 0),
		Vector2f(0, 0),
		Vector2f(0, 0),
	};

	void draw(RenderTarget *target, Vector2f swordOri, Vector2f swordTip) {
		
		Vertex line[] =
		{
			//Vertex(swordVertices[1].position, Color::White),
			Vertex(swordTip, Color::White),
			Vertex(Vector2f(0,0), Color::White)
		};
		

		Transform t;
		t.translate(swordOri);
		
		target->draw(swordVertices, SWORD_VERTEX_COUNT, sf::TrianglesFan, t);
		target->draw(line, 2, sf::Lines, t);
	}

	void update(float elapsedTime, Vector2f swordTip) {

		swordVertices[1].position = swordTip;
		// Follow sword edge
		for (int i = 2; i < SWORD_VERTEX_COUNT; i++) {
			approach(&swordVertices[i].position, swordVertices[i - 1].position, 20.0f * elapsedTime);
		}
	}
};


#endif
