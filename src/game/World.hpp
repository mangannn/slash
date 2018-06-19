#ifndef _World_
#define _World_

#include <vector>

class Object;
class Spawn;
class Player;

namespace World {

	extern std::vector<Object *> *objects;
	extern std::vector<Player *> *players;

	//std::vector<Spawn *> *spawns;

	extern std::vector<Object *> objectsToRemove;


	void add(Object *o);

	void remove(Object *o);

	void update();

	void addPlayer(Player *p);
	void addObject(Object *o);
};

#endif
