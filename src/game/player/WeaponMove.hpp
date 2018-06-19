#ifndef _WeaponMove_
#define _WeaponMove_

using namespace sf;

class WeaponMove {

public:

	// outreach of weapon
	float reachBegin, reachEnd;

	// direction relative character of weapon tip
	float dirBegin; float dirEnd;

	// weapon rotation from direction
	//float rotBegin; float rotEnd;

	// running velocity of wielder
	float velocity;

	float durutation;

	float timer;

	WeaponMove(float rB, float rE, float dB, float dE, float v, float dur):
		reachBegin(rB),
		reachEnd(rE),
		dirBegin(dB),
		dirEnd(dE),
		velocity(v),
		durutation(dur),
		timer(0)
	{}

	virtual ~WeaponMove() {}
	
	virtual bool update(float elapsedTime) {
		timer += elapsedTime;
		return (timer > durutation);
	}

	float getLength() {
		return approach(reachBegin, reachEnd, timer/durutation);
	}
	float getDirection() {
		return approach(dirBegin, dirEnd, timer/durutation);
	}

	float getVelocity() {
		return approach(velocity, 0, (timer/durutation)*(timer/durutation));
	}
};

#endif
