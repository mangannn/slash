
#include "World.hpp"

#include "obj/Object.hpp"

namespace World {

	std::vector<Object *> *objects;
	std::vector<Player *> *players;

	//std::vector<Spawn *> *spawns;


	std::vector<Object *> objectsToRemove;


	void add(Object *o) {
		objects->push_back(o);
	}

	void remove(Object *o) {
		objectsToRemove.push_back(o);
	}

	void update() {

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


	void addPlayer(Player *p) {
		players->push_back(p);
	}
	void addObject(Object *o) {
		objects->push_back(o);
	}
};

