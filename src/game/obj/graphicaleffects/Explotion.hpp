#ifndef _Explotion_H_
#define _Explotion_H_

#include "../GraphicalEffect.hpp"

class Explotion: public GraphicalEffect {

public:

	Explotion(Vector2f position):
		GraphicalEffect(position, "media/animation/Poff/poff", 9, 0.08)
	{
	}
};

#endif
