#ifndef _Orb_H_
#define _Orb_H_

#include "../Projectile.hpp"


class Orb: public Projectile {

public:

	Orb(Vector2f position, Vector2f velocity):
		Projectile(position, velocity, "media/images/orb.png")
	{
	}
};

#endif
