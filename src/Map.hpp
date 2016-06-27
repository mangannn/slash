#ifndef _MAP_H_
#define _MAP_H_

#include "Functions.hpp"
#include "obj/Image.hpp"
#include "obj/Spawn.hpp"

#include <iostream>
#include <fstream>

#include <string.h>

class Map {

public:

	sf::Texture mapTex;
	sf::Sprite mapSprite;


	unsigned int numBgs = 0;
	sf::Sprite bgs[420];

	unsigned int numStatic = 0;
	CollisionBox staticBoxes[500];

	unsigned int numImages = 0;
	cs::Image images[1000];


	unsigned int numSpawns = 0;
	Spawn spawns[100];
	

	Map() {


		std::string mapFilename;
		std::ifstream file ("media/start-map.txt");
		if (file.is_open()) {
			if (!getline(file, mapFilename)) {
				std::cout << "Unable to read settings file!\n";
				mapFilename = "media/maps/map.txt";
			}
			file.close();
		} else {
			std::cout << "Unable to read settings file!\n";
			mapFilename = "media/maps/map.txt";
		}

		std::cout << "Loading map: " << mapFilename << "\n";
		loadMap(mapFilename.c_str());


		/*for (unsigned int i = 1; i < numImages; i++) {
			std::cout << images[i].pos.y - images[i - 1].pos.y << "\n";
		}*/
	}


	void drawBackground(RenderTarget *target) {
		for (unsigned int i = 0; i < numBgs; i++) {
			target->draw(bgs[i]);
		}
	}

	void drawDebug(RenderTarget *target) {

		for (unsigned int i = 0; i < numStatic; i++) {
			//staticBoxes[i].draw(target);
		}
		for (unsigned int i = 0; i < numSpawns; i++) {
			spawns[i].draw(target);
		}
	}



	void initPlayers(int num) {

		int playerNum = 0;

		for (unsigned int i = 0; i < numSpawns; i++) {
			
			std::string name = spawns[i].type.substr(0, spawns[i].type.find(":"));

			if (name == "player") {
				World::addPlayer(new Player(spawns[i].pos, playerNum));
				playerNum += 1;

				if (playerNum >= num) {
					break;
				}
			}
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
			} else if (strcmp(str1, "bg") == 0) {

				num_back = sscanf(str2, 
					"%[^,\n],%f,%f", 
					str1, &x, &y);

				if (num_back != 3) {
					continue;
				}

				sf::Texture *texture = Resources::getTexture(str1);
				bgs[numBgs].setTexture(*texture);
				bgs[numBgs].setOrigin(sf::Vector2f((float)texture->getSize().x, (float)texture->getSize().y) / 2.0f);
				bgs[numBgs].setPosition(x, y);

				numBgs += 1;

			} else if (strcmp(str1, "spawn") == 0) {

				num_back = sscanf(str2, 
					"%[^,\n],%f,%f,%f", 
					str1, &x, &y, &r);

				if (num_back != 4) {
					continue;
				}


				spawns[numSpawns] = Spawn(Vector2f(x, y), str1, r);
				numSpawns += 1;
			}
		}
		fclose(file);
	}

};

#endif
