#ifndef _MAP_H_
#define _MAP_H_

#include "Functions.hpp"
#include "obj/Image.hpp"
#include "obj/Spawn.hpp"
#include "obj/enemies/Imp.hpp"

#include <iostream>
#include <fstream>

#include <string.h>

class Map {

public:

	sf::Texture mapTex;
	sf::Sprite mapSprite;


	unsigned int numBgs = 0;
	sf::Sprite bgs[500];

	unsigned int numStatic = 0;
	CollisionBox staticBoxes[500];

	unsigned int numCollisionLines = 0;
	CollisionLine collisionLines[500];

	unsigned int numImages = 0;
	cs::Image images[1000];

	std::vector<Spawn> spawns;
	

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
	}


	void drawBackground(RenderTarget *target) {
		for (unsigned int i = 0; i < numBgs; i++) {
			target->draw(bgs[i]);
		}
	}

#ifdef DEBUG
	void drawDebug(RenderTarget *target) {

		for (unsigned int i = 0; i < numStatic; i++) {
			staticBoxes[i].drawDebug(target);
		}
		for (unsigned int i = 0; i < numCollisionLines; i++) {
			collisionLines[i].drawDebug(target);
		}
		for (unsigned int i = 0; i < spawns.size(); i++) {
			spawns.at(i).drawDebug(target);
		}
	}
#endif

	void init(int numberPlayers) {

		int playerNum = 0;

		for (unsigned int i = 0; i < spawns.size(); i++) {
			
			std::string name = spawns.at(i).type.substr(0, spawns.at(i).type.find(":"));
			std::string arg1 = spawns.at(i).type.substr(spawns.at(i).type.find(":") + 1);


			if (name == "player") {

				if (playerNum < numberPlayers) {
					int actionButton[3] = {sf::Keyboard::Z, sf::Keyboard::X, sf::Keyboard::Space};
					Controls *con = new KeyboardControls(sf::Keyboard::Up, sf::Keyboard::Down, sf::Keyboard::Left, sf::Keyboard::Right, actionButton, 2);
					World::addPlayer(new Player(spawns.at(i).pos, con));
				}

				playerNum += 1;

				spawns.erase(spawns.begin() + i);
				i -= 1;
			} else if (name == "playerdummy") {

				Controls *con = new DummyControls(atoi(arg1.c_str()));
				World::addPlayer(new Player(spawns.at(i).pos, con));

				playerNum += 1;
				
				spawns.erase(spawns.begin() + i);
				i -= 1;
			} else if (name == "imp") {

				int type = atoi(arg1.c_str());

				World::addObject(new Imp(spawns.at(i).pos, type));

				spawns.erase(spawns.begin() + i);
				i -= 1;
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

		float x = 0, y = 0, r = 0, x2 = 0, y2 = 0;

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
			} else if (strcmp(str1, "line") == 0) {

				num_back = sscanf(str2, 
					"%f,%f,%f,%f", 
					&x, &y, &x2, &y2);

				if (num_back != 4) {
					continue;
				}

				collisionLines[numCollisionLines] = CollisionLine(Vector2f(x, y), Vector2f(x2, y2));
				numCollisionLines += 1;
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


				spawns.push_back(Spawn(Vector2f(x, y), str1, r));
			}
		}
		fclose(file);
	}

};

#endif
