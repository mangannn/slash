#ifndef _COLL_H_
#define _COLL_H_

#include "obj/Object.hpp"
#include "Functions.hpp"



class CollisionBox {

public:

	Vector2f *offset;

	Vector2f pos;
	float r;

	CollisionBox() {}
	CollisionBox(Vector2f *offsetParam, Vector2f position, float radius):
		offset(offsetParam),
		pos(position),
		r(radius)
	{
	}

	static bool check(CollisionBox a, CollisionBox b) {
		float coll_dist = a.r + b.r;
		Vector2f diff = a.getPosition() - b.getPosition();

		if (sqrSize(diff) < coll_dist * coll_dist) {
			return true;
		}
		return false;
	}

	void draw(RenderTarget *target) {

		// for debug
		sf::CircleShape shape(r);
		shape.setFillColor(sf::Color(0, 0, 0, 0));
		shape.setOutlineThickness(1);
		shape.setOutlineColor(sf::Color(250, 150, 100));
		shape.setPosition(getPosition() - Vector2f(r, r));
		target->draw(shape);
	}

	Vector2f getPosition() {
		return (*offset) + pos;
	}
};


class CollisionLine {

private:

	Vector2f line_dir;
	float line_length;

public:

	Vector2f p1, p2;

	CollisionLine() {}
	CollisionLine(Vector2f pos1, Vector2f pos2):
		p1(pos1),
		p2(pos2)
	{
		line_dir = (p2 - p1);
		line_length = size(line_dir);
		line_dir = (1.f/line_length)*line_dir;
	}

	Vector2f checkCollision(CollisionBox c) {

		float along_line = dot((c.getPosition() - p1), line_dir);

		Vector2f collision_vec(0,0);
		bool check_collision_distanse = false;

		if (along_line < 0) {
			if (along_line > -c.r) {
				// maybe collide with p1
				collision_vec = c.getPosition() - p1;
				check_collision_distanse = true;
			}
		} else if (along_line > line_length) {
			if (along_line < line_length + c.r) {
				// maybe collide with p2
				collision_vec = c.getPosition() - p2;
				check_collision_distanse = true;
			}
		} else {
			// maybe collide with line
			collision_vec = c.getPosition() - ((along_line * line_dir) + p1);
			check_collision_distanse = true;
		}

		if (check_collision_distanse) {

			float collision_dist = size(collision_vec);
			
			if (collision_dist < c.r) {
				// collision occured
				Vector2f correction_vec = ((c.r/collision_dist) - 1) * collision_vec;
				return correction_vec;
			}
		}

		// no collision occured
		return Vector2f(0, 0);
	}

	void draw(RenderTarget *target) {


		Vector2f d = 10.f * Vector2f(line_dir.y, -line_dir.x);

		sf::Vertex line[] =
		{

		    sf::Vertex(p1),
		    sf::Vertex(p2),
		    sf::Vertex(p1),
			sf::Vertex(p1+d),
		    sf::Vertex(0.5f*(p1+p2)),
			sf::Vertex((0.5f*(p1+p2))+(0.5f*d)),
		    sf::Vertex(p2),
		    sf::Vertex(p2+d)
		};

		target->draw(line, 8, sf::Lines);
	}
};

#endif
