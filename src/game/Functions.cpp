#include "Functions.hpp"

#include <math.h>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;

float frac(float x) {
	return x - floor(x);
}

bool isZero(sf::Vector2f v) {
	return (v.x == 0) && (v.y == 0);
}
float size(sf::Vector2f v) {
	return sqrt((v.x * v.x) + (v.y * v.y));
}
float sqrSize(sf::Vector2f v) {
	return (v.x * v.x) + (v.y * v.y);
}

float dot(sf::Vector2f a, sf::Vector2f b) {
	return (a.x * b.x) + (a.y * b.y);
}
float semiCross(sf::Vector2f a, sf::Vector2f b) {
	return (a.x * b.y) - (a.y * b.x);
}

float angle(sf::Vector2f v) {
	return (atan2(v.y, v.x) * 180.0f) / M_PI;
}

float cutToPeriod(float v, float periodStart, float periodEnd) {
	while (v >= periodEnd) {v -= periodEnd - periodStart;}
	while (v < periodStart) {v += periodEnd - periodStart;}
	return v;
}

float periodValueBetween(float angle, float target, float percent, float period) {
	angle = cutToPeriod(angle, - period / 2, period / 2);
	float diffrence = cutToPeriod(target - angle, - period / 2, period / 2);
	return cutToPeriod(diffrence * percent + angle, 0, period);
}


float approach(float current, float target, float percent) {
	return (target - current) * percent + current;;
}

Vector2f approach(Vector2f current, Vector2f target, float percent) {
	approach(&current, target, percent);
	return current;
}

void approach(Vector2f *current, Vector2f target, float percent) {
	*current = (target - *current) * percent + *current;
}


float	towardsAngle(float current, float goal, float amount) {

	if (current - goal > M_PI) {
		current -= M_PI * 2.f;
	} else if (-(current - goal) > M_PI) {
		current += M_PI * 2.f;
	}

	if (current < goal) {
		current += amount;
		if (current > goal) {
			current = goal;
		}
	} else {
		current -= amount;
		if (current < goal) {
			current = goal;
		}
	}

	return current;
}

float	decrese(float current, float amount) {
	if (current < amount) {
		return 0;
	} else {
		return current  - amount;
	}
}
float	increse(float current, float amount, float max) {
	if (current > max - amount) {
		return max;
	} else {
		return current + amount;
	}
}

Vector2f	changeVectorSize(Vector2f current, float amount, float max) {
	if (amount > 0) {
		return increse(current, amount, max);
	} else {
		return decrese(current, -amount);
	}
}
Vector2f	decrese(Vector2f current, float amount) {
	float s = size(current);
	if (s < amount) {
		return Vector2f(0,0);
	} else {
		return current * (1 - (amount/s));
	}
}
Vector2f	increse(Vector2f current, float amount, float max) {
	float s = size(current);
	if (s == 0.f) {
		return Vector2f(0,0);
	} else if (s > max - amount) {
		return current * (max/s);;
	} else {
		return current * (1 + (amount/s));
	}
}

int discreteDirectionFromVectorEight(Vector2f a) {
	float absX = abs(a.x);
	float absY = abs(a.y);

	if (absX > absY) {
		// vertical side
		float half = absX * 0.4142;
		if (a.x > 0) {
			// left side
			if (a.y > half) return 1; // south-east
			if (a.y < -half) return 7; // north-east
			return 0; // east
		} else {
			// right side
			if (a.y > half) return 3; // south-west
			if (a.y < -half) return 5; // north-west
			return 4; // west
		}
	} else {
		// horisontal side
		float half = absY * 0.4142;
		if (a.y > 0) {
			// bottom
			if (a.x > half) return 1; // south-east
			if (a.x < -half) return 3; // south-west
			return 2; // south
		} else {
			// top
			if (a.x > half) return 7; // north-east
			if (a.x < -half) return 5; // north-west
			return 6; // north
		}
	}
}

int discreteDirectionFromVectorFour(Vector2f a) {
	float absX = abs(a.x);
	float absY = abs(a.y);

	if (absX > absY) {
		// vertical side
		if (a.x > 0) {
			// left side
			return 0; // east
		} else {
			// right side
			return 2; // west
		}
	} else {
		// horisontal side
		if (a.y > 0) {
			// bottom
			return 1; // south
		} else {
			// top
			return 3; // north
		}
	}
}

int discreteDirectionFromAngle(float angle, int disc) {

	float intervall = 2.f * M_PI / disc;

	while (angle < 0) {
		angle += 2.f * M_PI;
	}

	angle += intervall / 2.f;

	int dir = 0;
	while (angle > intervall) {
		angle -= intervall;
		dir++;
	}
	while (dir >= disc) {
		dir -= disc;
	}
	return dir;
}

float angleFromDiscreteDirection(int dir, int disc) {
	return (2.f / (float)disc) * M_PI * (float)dir;
}

Vector2f vectorFromDiscreteDirectionEight(int dir) {
	switch (dir) {
		case 0: return Vector2f(1,0);
		case 1: return Vector2f(M_SQRT1_2,M_SQRT1_2);
		case 2: return Vector2f(0,1);
		case 3: return Vector2f(-M_SQRT1_2,M_SQRT1_2);
		case 4: return Vector2f(-1,0);
		case 5: return Vector2f(-M_SQRT1_2,-M_SQRT1_2);
		case 6: return Vector2f(0,-1);
		case 7: return Vector2f(M_SQRT1_2,-M_SQRT1_2);
		default: return Vector2f(0,0);
	}
}


bool lineIntersect(Vector2f a1, Vector2f a2, Vector2f b1, Vector2f b2) {
	
	Vector2f p = a1, r = a2 - a1, q = b1, s = b2 - b1;

	float n = semiCross(r, s);

	if (n != 0) {

		float l = semiCross(q - p, r) / n;
		float u = semiCross(q - p, s) / n;

		if (fmin(l, u) >= 0 && fmax(l, u) <= 1) {
			return true;
		}
	} else if (semiCross(q - p, r) == 0) {
		// rare case when lines are parallel and might be intersecting

		Vector2f rd = r / sqrSize(r);

		float t0 = dot((q - p), rd);
		float t1 = t0 + dot(s, rd);

		if (fmax(t0, t1) >= 0 && fmin(t0, t1) <= 1) {
			return true;
		}
	}

	return false;
}


void lineIntersectDebug() {

	std::cout << "true" << std::endl;

	float t[] = {
		1,0, -1,0, 0,1, 0,-1,
		-1,0, 1,0, 0,1, 0,-1,
		0,1, 0,-1, 1,0, -1,0,
		0,1, 0,-1, -1,0, 1,0,
		1,0, -1,0, 0,1, 1,-1,
		-1,0, 1,0, 0,1, 1,-1,
		0,1, 1,-1, 1,0, -1,0,
		0,1, 1,-1, -1,0, 1,0,
		1,0, -1,1, 0,1, 0,-1,
		-1,0, 1,1, 0,1, 0,-1,
		0,1, 0,-1, 1,0, -1,1,
		0,1, 0,-1, -1,0, 1,1,
		1,0, 0,1, 0,0, 1,1,
		0,1, 1,0, 0,0, 1,1,
		0,0, 1,1, 1,0, 0,1,
		0,0, 1,1, 0,1, 1,0
	};

	for (int i = 0; i < 16; i++) {

		std::cout << "intersection: " << lineIntersect(
			Vector2f(t[8 * i + 0],t[8 * i + 1]), Vector2f(t[8 * i + 2],t[8 * i + 3]),
			Vector2f(t[8 * i + 4],t[8 * i + 5]), Vector2f(t[8 * i + 6],t[8 * i + 7]) 
			) << std::endl;
	}

	std::cout << "false" << std::endl;

	float f[] = {
		2, 3, 3, 2, 0, 0, 1, 1,
		3, 2, 2, 3, 0, 0, 1, 1,
		0, 0, 1, 1, 2, 3, 3, 2, 
		0, 0, 1, 1, 3, 2, 2, 3, 
		0.967674, -29.9809, -3.08349, -30.4089, 0, 80, 0, 0,
		-146.176, -97.4457, -146.176, -97.4457, -147.019, -2.70507, 10.7836, 33.6048,
		0, 25, 0, 25, 80, 0, 0, 0,
		-71.7272, -68.7826, -71.7272, -68.7826, -74.2051, -3.98992, -66.5042, 93.0426,
		-71.7272, -68.7826, -71.7272, -68.7826, -70.0826, 123.088, -115.216, 3.1257,
		4.59559, -62.3794, 4.59559, -62.3794, 41.6498, -133.101, -4.59137, -98.1135,

		-29.9809, 0.967674, -30.4089, -3.08349, 80, 0, 0, 0, 
		-97.4457, -146.176, -97.4457, -146.176, -2.70507, -147.019, 33.6048, 10.7836, 
		25, 0, 25, 0, 0, 80, 0, 0, 
		-68.7826, -71.7272, -68.7826, -71.7272, -3.98992, -74.2051, 93.0426, -66.5042, 
		-68.7826, -71.7272, -68.7826, -71.7272, 123.088, -70.0826, 3.1257, -115.216, 
		-62.3794, 4.59559, -62.3794, 4.59559, -133.101, 41.6498, -98.1135, -4.59137,

		0, 25, 0, 24.7172, 80, 0, 0, 0,
		25, 0, 24.7172, 0, 0, 80, 0, 0
	};

	for (int i = 0; i < 18; i++) {

		std::cout << "intersection: " << lineIntersect(
			Vector2f(f[8 * i + 0],f[8 * i + 1]), Vector2f(f[8 * i + 2],f[8 * i + 3]),
			Vector2f(f[8 * i + 4],f[8 * i + 5]), Vector2f(f[8 * i + 6],f[8 * i + 7]) 
			) << std::endl;
		
	}

}


Color tintColor(Color c, int amount) {
	c.r += 10 * amount;
	c.g += 5 *amount;
	c.b += -30 * amount;
	return c;
}
