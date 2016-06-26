#ifndef _Flash_H_
#define _Flash_H_

#include "GraphicalEffect.hpp"


class Flash: public GraphicalEffect {

public:

	Flash(Vector2f position):
		GraphicalEffect(position, "media/ani/flash", 8, 0.04)
	{
	}
};

#endif
