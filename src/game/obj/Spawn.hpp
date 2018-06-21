#ifndef _Spawn_
#define _Spawn_

#include "Object.hpp"

class Spawn: public Object {

public:

	std::string type;

	float timer = 0;
	float spawnIntervall = 4.9;


	float what;

	Spawn():
		Object(Vector2f(0,0))
	{}
	Spawn(Vector2f position, std::string typeParam);

	virtual void update(float elapsedTime);

	void spawnAction();

#ifdef DEBUG
	virtual void drawDebug(RenderTarget *target);
#endif

};

#endif
