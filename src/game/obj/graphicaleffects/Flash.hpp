#ifndef _Flash_H_
#define _Flash_H_

#include "../GraphicalEffect.hpp"


class Flash: public GraphicalEffect {

public:

	Flash(Vector2f position):
		GraphicalEffect(position, "media/animation/Flash/flash", 8, 0.03)
	{
	}
};

#endif
