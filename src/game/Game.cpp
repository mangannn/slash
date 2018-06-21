
#include <math.h>
#include <iostream>
#include <vector>

#include "Resources.hpp"
#include "Functions.hpp"

#include "Game.hpp"

#include "obj/Object.hpp"
#include "player/Player.hpp"
#include "Map.hpp"

#include "obj/Enemy.hpp"


#include "Collision.hpp"

#include "World.hpp"




#define PARTICLE_DESPAWN_DIST (1000.0f)
#define ENEMY_DESPAWN_DIST (10000.0f)



Game::Game() {

	World::players = new std::vector<Player *>();
	World::objects = new std::vector<Object *>();


	map = new Map();

	map->init(1 /* one player */);

	gameView.setSize(Vector2f(1000, 1000));
	gameView.setCenter(Vector2f(0,0));;
}
Game::~Game() {

	delete map;

	{
		Object *temp;
		while (!World::objects->empty()) {
			temp = World::objects->back();
			delete temp;
			World::objects->pop_back();
		}
		delete World::objects;
	}
	{
		Object *temp;
		while (!World::players->empty()) {
			temp = World::players->back();
			delete temp;
			World::players->pop_back();
		}
		delete World::players;
	}
}



void Game::eventHandle(sf::Event event) {
	for (unsigned int i = 0; i < World::players->size(); i++) {
		World::players->at(i)->controls->eventHandle(event);
	}

	switch (event.type) {
		case sf::Event::KeyPressed: {

			switch (event.key.code) {
				case sf::Keyboard::Return: {

				} break;
				default: break;
			}
		} break;
		default: break;
	}
}







void Game::update(float elapsedTime) {


	for (unsigned int i = 0; i < map->spawns.size(); i++) {
		map->spawns.at(i).update(elapsedTime);
	}

	for (unsigned int i = 0; i < World::objects->size(); i++) {
		World::objects->at(i)->update(elapsedTime);
	}
	for (unsigned int i = 0; i < World::players->size(); i++) {
		World::players->at(i)->update(elapsedTime);
	}


	// despawn particles
	for (unsigned int i = 0; i < World::objects->size(); i++) {
		if (dynamic_cast<Projectile *>(World::objects->at(i)) != NULL) {
			if (sqrSize(World::objects->at(i)->pos - gameView.getCenter()) > PARTICLE_DESPAWN_DIST * PARTICLE_DESPAWN_DIST) {
				World::remove(World::objects->at(i));
			}
		}
	}


	Character *ch;
	Character *ch1;
	Character *ch2;
	Orb *orb;

	Vector2f temp;

	// Collisions with lines
	for (unsigned int i = 0; i < map->numCollisionLines; i++) {
		for (unsigned int j = 0; j < World::players->size(); j++) {
			if (!isZero(temp = map->collisionLines[i].checkCollision(World::players->at(j)->walkBox))) {
				World::players->at(j)->pos += temp;
			}
		}
		for (unsigned int j = 0; j < World::objects->size(); j++) {
			if ((ch = dynamic_cast<Character *>(World::objects->at(j))) != NULL) {
				if (!isZero(temp = map->collisionLines[i].checkCollision(ch->walkBox))) {
					ch->pos += temp;
				}
			}
		}
	}

	// Collisions with cicles
	for (unsigned int i = 0; i < map->numStatic; i++) {
		for (unsigned int j = 0; j < World::players->size(); j++) {
			if (CollisionBox::check(World::players->at(j)->walkBox, map->staticBoxes[i])) {
				float coll_dist = World::players->at(j)->walkBox.r + map->staticBoxes[i].r;
				Vector2f diff = World::players->at(j)->walkBox.getPosition() - map->staticBoxes[i].getPosition();
				World::players->at(j)->pos += diff * ((coll_dist / size(diff)) - 1.f);
			}
		}
		for (unsigned int j = 0; j < World::objects->size(); j++) {
			if ((ch = dynamic_cast<Character *>(World::objects->at(j))) != NULL) {
				if (CollisionBox::check(ch->walkBox, map->staticBoxes[i])) {

					float coll_dist = ch->walkBox.r + map->staticBoxes[i].r;
					Vector2f diff = ch->walkBox.getPosition() - map->staticBoxes[i].getPosition();
					ch->pos += diff * ((coll_dist / size(diff)) - 1.f);
				}
			}
		}
	}

	// Collisions character with characters
	for (unsigned int i = 0; i < World::objects->size(); i++) {
		if ((ch1 = dynamic_cast<Character *>(World::objects->at(i))) != NULL) {
			for (unsigned int j = i + 1; j < World::objects->size(); j++) {
				if ((ch2 = dynamic_cast<Character *>(World::objects->at(j))) != NULL) {
					if (CollisionBox::check(ch1->walkBox, ch2->walkBox)) {

						float coll_dist = ch1->walkBox.r + ch2->walkBox.r;
						Vector2f diff = ch1->walkBox.getPosition() - ch2->walkBox.getPosition();
						ch1->pos += diff * (0.5f * ((coll_dist / size(diff)) - 1.f));
						ch2->pos -= diff * (0.5f * ((coll_dist / size(diff)) - 1.f));
					}
				}
			}
		}
	}

	// Collisions player with characters
	for (unsigned int j = 0; j < World::players->size(); j++) {
		for (unsigned int i = 0; i < World::objects->size(); i++) {
			if ((ch = dynamic_cast<Character *>(World::objects->at(i))) != NULL) {
				if (CollisionBox::check(ch->walkBox, World::players->at(j)->walkBox)) {

					float coll_dist = ch->walkBox.r + World::players->at(j)->walkBox.r;
					Vector2f diff = ch->walkBox.getPosition() - World::players->at(j)->walkBox.getPosition();
					ch->pos += diff * (0.5f * ((coll_dist / size(diff)) - 1.f));
					World::players->at(j)->pos -= diff * (0.5f * ((coll_dist / size(diff)) - 1.f));
				}
			}
		}
	}



	Vector2f tempVector;


	for (unsigned int j = 0; j < World::players->size(); j++) {

		for (unsigned int i = 0; i < World::objects->size(); i++) {

			if ((orb = dynamic_cast<Orb *>(World::objects->at(i))) != NULL) {
				if (World::players->at(j)->weapon->canParry() && World::players->at(j)->weapon->isHitCircle(orb->box, &tempVector)) {

					orb->vel += tempVector * 10.f;

					World::add(new Flash(orb->pos));
					World::players->at(j)->weapon->getParried(-tempVector);

				} else if (CollisionBox::check(World::players->at(j)->bodyBox, orb->box)) {
					//std::cout << "Auuu!\n";
				}
			} else if ((ch = dynamic_cast<Character *>(World::objects->at(i))) != NULL) {
				
				if (ch->weapon->canHurt() && World::players->at(j)->weapon->canGetHurt()) {
					if (ch->weapon->isHitCircle(World::players->at(j)->bodyBox, &tempVector)) {
						World::players->at(j)->hit(tempVector);
					}
				}
				if (ch->weapon->canGetHurt() && World::players->at(j)->weapon->canHurt()) {
					if (World::players->at(j)->weapon->isHitCircle(ch->bodyBox, &tempVector)) {
						ch->hit(tempVector);
					}
				}

				if (World::players->at(j)->weapon->canParry() && ch->weapon->canBeParried()) {
					if (World::players->at(j)->weapon->isHitCircle(ch->weapon->getParryBox(), &tempVector)) {
						World::add(new Flash(ch->weapon->getParryBox().getPosition()));

						ch->weapon->getParried(tempVector);
					}
				}
			}
		}
	}


	World::update();
}


// this is not member of Game, very ugly, but later fix
int insertByDepth(std::vector<Object *> *list, Object *o) {
	for (unsigned int i = 0; i < list->size(); i++) {
		if (o->pos.y < list->at(i)->pos.y) {
			list->insert(list->begin() + i, o);
			return i;
		}
	}

	list->push_back(o);
	return list->size() - 1;
}



void Game::draw(RenderTarget *target) {

	target->clear();

    Vector2u targetSize = target->getSize();
	float aspect = ((float)targetSize.x / (float)targetSize.y);

	// set game view
	{

		std::vector<Player *> *followedObjects = World::players;

		Vector2f smallest_most = followedObjects->at(0)->pos;
		Vector2f largest_most = followedObjects->at(0)->pos;
		for (unsigned int i = 1; i < followedObjects->size(); i++) {
			Vector2f v = followedObjects->at(i)->pos;
			if (v.x > largest_most.x) {
				largest_most.x = v.x;
			} else if (v.x < smallest_most.x) {
				smallest_most.x = v.x;
			}
			if (v.y > largest_most.y) {
				largest_most.y = v.y;
			} else if (v.y < smallest_most.y) {
				smallest_most.y = v.y;
			}
		}


		// 600
		float scale_multiply = size(smallest_most - largest_most) + 250.0f;

		// 600
		if (scale_multiply < 250.0f) {
			scale_multiply = 250.0f;
		}

		Vector2f newPosition = (smallest_most + largest_most) / 2.0f;
		Vector2f currentPosition = gameView.getCenter();

		gameView.setCenter((newPosition - currentPosition) / 4.0f + currentPosition);

		float newMultiply = scale_multiply;
		float currentMultiply = gameView.getSize().y;

		//float aspect = ((float)gamePixelArea.getSize().x / (float)gamePixelArea.getSize().y);

		gameView.setSize(Vector2f(aspect, 1.0f) * ((newMultiply - currentMultiply) / 4.0f + currentMultiply));
		
		target->setView(gameView);

	}

	map->drawBackground(target);


	std::vector<Object *> sorted; // sorted by depth

	for (unsigned int i = 0; i < World::players->size(); i++) {
		insertByDepth(&sorted, World::players->at(i));
	}
	for (unsigned int i = 0; i < World::objects->size(); i++) {
		insertByDepth(&sorted, World::objects->at(i));
	}


	unsigned int movable_index = 0;

	for (unsigned int i = 0; i < map->numImages; i++) {
		while (movable_index < sorted.size() && sorted.at(movable_index)->pos.y < map->images[i].pos.y) {
			sorted.at(movable_index)->draw(target);
			movable_index += 1;
		}
		map->images[i].draw(target);
	}

    for (unsigned int i = movable_index; i < sorted.size(); i++) {
		sorted.at(i)->draw(target);
	}


	// only for debug
	map->drawDebug(target);
}

