#ifndef _World_
#define _World_

#include <vector>

class Object;
class Spawn;
class Player;

namespace World {

	extern std::vector<Object *> *objects;
	extern std::vector<Player *> *players;

	extern std::vector<Spawn *> *spawns;

	void update();

	void add(Object *o);
	void addPlayer(Player *p);
	void addSpawn(Spawn *o);

	void remove(Object *o);
	void removeSpawn(Spawn *o);
};

#endif
