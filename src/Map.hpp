#ifndef _MAP_H_
#define _MAP_H_

#include "Functions.hpp"
#include "obj/Image.hpp"

#include <iostream>

#include <string.h>

class Map {

public:

	sf::Texture mapTex;
	sf::Sprite mapSprite;


	unsigned int numStatic = 0;
	CollisionBox staticBoxes[1000];

	unsigned int numImages = 0;
	cs::Image images[1000];
	

	Map() {

		loadMap("media/maps/forest2.txt");

		/*for (unsigned int i = 1; i < numImages; i++) {
			std::cout << images[i].pos.y - images[i - 1].pos.y << "\n";
		}*/
    
		mapTex.loadFromFile("media/maps/forest.png");
		mapSprite.setTexture(mapTex);
		mapSprite.setOrigin(sf::Vector2f((float)mapTex.getSize().x / 2.0f, (float)mapTex.getSize().y / 2.0f));
	}


	void drawBackground(RenderTarget *target) {
		target->draw(mapSprite);
	}

	void draw(RenderTarget *target) {

		for (unsigned int i = 0; i < numStatic; i++) {
			//staticBoxes[i].draw(target);
		}
	}

private:

	void loadMap(const char* path) {


		Vector2f *origo = new Vector2f(0,0);


		FILE *file;

		if (!(file = fopen(path, "r"))) {
			std::cout << "Failed to open file: " << path << std::endl;
			exit(-1);
		}

		const int MAXSTR = 256;
		char buffer[MAXSTR];

		char str1[MAXSTR];
		char str2[MAXSTR];

		float x = 0, y = 0, r = 0;

		int num_back;

		while (fgets(buffer, MAXSTR, file)) {

			num_back = sscanf(buffer, 
				"%[^{\n]{%[^}\n]}", 
				str1, str2);

			if (num_back != 2) {
				continue;
			}

			if (strcmp(str1, "box") == 0) {

				num_back = sscanf(str2, 
					"%f,%f,%f", 
					&x, &y, &r);

				if (num_back != 3) {
					continue;
				}

				staticBoxes[numStatic] = CollisionBox(origo, Vector2f(x, y), r);
				numStatic += 1;
			} else if (strcmp(str1, "img") == 0) {

				num_back = sscanf(str2, 
					"%[^,\n],%f,%f", 
					str1, &x, &y);

				if (num_back != 3) {
					continue;
				}

				images[numImages] = cs::Image(Vector2f(x, y), str1);
				numImages += 1;
			}
		}
		fclose(file);
	}

};

#endif
