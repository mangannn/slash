
#include "World.hpp"

#include "obj/Object.hpp"
#include "obj/Spawn.hpp"

namespace World {

	std::vector<Object *> *objects;
	std::vector<Player *> *players;

	std::vector<Spawn *> *spawns;


	std::vector<Object *> objectsToRemove;

	std::vector<Spawn *> spawnsToRemove;

	void update() {

		{
			Object *temp;
			while (!objectsToRemove.empty()) {
				temp = objectsToRemove.back();

				for (unsigned int i = 0; i < objects->size(); i++) {
					if (temp == objects->at(i)) {
						objects->erase(objects->begin() + i);
						break;
					}
				}

				delete temp;
				objectsToRemove.pop_back();
			}
		}
		{
			Spawn *temp;
			while (!spawnsToRemove.empty()) {
				temp = spawnsToRemove.back();

				for (unsigned int i = 0; i < spawns->size(); i++) {
					if (temp == spawns->at(i)) {
						spawns->erase(spawns->begin() + i);
						break;
					}
				}

				delete temp;
				spawnsToRemove.pop_back();
			}
		}
	}



	void add(Object *o) {
		objects->push_back(o);
	}

	void addPlayer(Player *p) {
		players->push_back(p);
	}
	void addSpawn(Spawn *o) {
		spawns->push_back(o);
	}

	void remove(Object *o) {
		objectsToRemove.push_back(o);
	}
	void removeSpawn(Spawn *o) {
		spawnsToRemove.push_back(o);
	}
};

