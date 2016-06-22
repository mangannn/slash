#ifndef _MAP_H_
#define _MAP_H_

#include "Functions.hpp"

class Map {

public:

	sf::Texture mapTex;
	sf::Sprite mapSprite;


	const static int numStatic = 7;
	CollisionBox staticBoxes[numStatic] = {
		CollisionBox(NULL, Vector2f(100, 100), 50),
		CollisionBox(NULL, Vector2f(-40, 240), 130),

		CollisionBox(NULL, Vector2f(-400, 10), 300),
		CollisionBox(NULL, Vector2f(-100, -250), 100),
		CollisionBox(NULL, Vector2f(300, -220), 120),

		CollisionBox(NULL, Vector2f(200, 220), 90),
		CollisionBox(NULL, Vector2f(250, 20), 130)
	};
	

	Map() {

		Vector2f *origo = new Vector2f(0,0);

		for (int i = 0; i < numStatic; i++) {
			staticBoxes[i].offset = origo;
		}


    
		mapTex.loadFromFile("media/images/map.png");
		mapSprite.setTexture(mapTex);
		mapSprite.setOrigin(sf::Vector2f((float)mapTex.getSize().x / 2.0f, (float)mapTex.getSize().y / 2.0f));
		mapSprite.setScale(8.0f, 8.0f);
	}

	void draw(RenderTarget *window) {

		window->draw(mapSprite);

		for (int i = 0; i < numStatic; i++) {
			staticBoxes[i].draw(window);
		}
	}

};

#endif
