#include <math.h>
#include <iostream>

#include "Resources.hpp"
#include "Functions.hpp"

#include "Game.hpp"

#include "IvansTestAni/head.hpp"
#include "IvansTestAni/titelEffekt.hpp"


#include "Object.hpp"
#include "Player.hpp"
#include "Map.hpp"

#include "obj/Enemy.hpp"
#include "obj/Explotion.hpp"
#include "obj/Flash.hpp"


#include "Collision.hpp"

#include "World.hpp"




#define PARTICLE_TRACKING_DIST (1000.0f)



Game::Game() {

	World::players = new std::vector<Player *>();
	World::objects = new std::vector<Object *>();


	map = new Map();

	int numJoysticks = 0; 

	for (int i = 0; i < 4; i++) {
		if (sf::Joystick::isConnected(i)) {
			numJoysticks += 1;
		} else {
			break;
		}
	}

	if (numJoysticks == 0) {
		numJoysticks = 1;
	}

	map->initPlayers(numJoysticks);


    //World::add(new RotAni(Vector2f(0,-10)));
    //World::add(new TitelEffekt(Vector2f(120,100)));



    scaleFactor = 3.0;
    
    gamePixelArea.create(320, 240);
    monitorPixelArea.create((int)(gamePixelArea.getSize().x * scaleFactor), (int)(gamePixelArea.getSize().y * scaleFactor));
    
    //gamePixelArea.setSmooth(false);
    




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

	switch (event.type) {
		case sf::Event::KeyPressed: {

			switch (event.key.code) {
				case sf::Keyboard::Return: {

				} break;
				case sf::Keyboard::A: {

				} break;
				default: break;
			}
		} break;
		default: break;
	}
}







void Game::update(float elapsedTime) {


	for (unsigned int i = 0; i < map->numSpawns; i++) {
		map->spawns[i].update(elapsedTime);
	}

	for (unsigned int i = 0; i < World::objects->size(); i++) {
		World::objects->at(i)->update(elapsedTime);
	}
	for (unsigned int i = 0; i < World::players->size(); i++) {
		World::players->at(i)->update(elapsedTime);
	}


	for (unsigned int i = 0; i < World::objects->size(); i++) {
		if (dynamic_cast<Projectile *>(World::objects->at(i)) != NULL) {
			if (sqrSize(World::objects->at(i)->pos - gameView.getCenter()) > PARTICLE_TRACKING_DIST * PARTICLE_TRACKING_DIST) {
				World::remove(World::objects->at(i));
			}
		}
	}


	Enemy *e;
	Orb *orb;


	// Collisions
	for (unsigned int i = 0; i < map->numStatic; i++) {
		for (unsigned int j = 0; j < World::players->size(); j++) {
			if (CollisionBox::check(World::players->at(j)->walkBox, map->staticBoxes[i])) {
				float coll_dist = World::players->at(j)->walkBox.r + map->staticBoxes[i].r;
				Vector2f diff = World::players->at(j)->walkBox.getPosition() - map->staticBoxes[i].getPosition();
				World::players->at(j)->pos += diff * (coll_dist / size(diff) - 1);
			}
		}

		for (unsigned int j = 0; j < World::objects->size(); j++) {

			if ((e = dynamic_cast<Enemy *>(World::objects->at(j))) != NULL) {
				if (CollisionBox::check(e->box, map->staticBoxes[i])) {

					float coll_dist = e->box.r + map->staticBoxes[i].r;
					Vector2f diff = e->box.getPosition() - map->staticBoxes[i].getPosition();
					e->pos += diff * (coll_dist / size(diff) - 1);
				}
			}
		}
	}

	for (unsigned int j = 0; j < World::players->size(); j++) {

		for (unsigned int i = 0; i < World::objects->size(); i++) {

			if ((orb = dynamic_cast<Orb *>(World::objects->at(i))) != NULL) {
				if (CollisionBox::check(World::players->at(j)->swordBox, orb->box)) {

					Vector2f diff = World::players->at(j)->swordBox.getPosition() - orb->pos;
					orb->vel = orb->vel - 2 * (dot(orb->vel, diff) / sqrSize(diff)) * diff;

					//World::add(new Flash((World::players->at(j)->swordBox.getPosition() + orb->pos) * 0.5f));
					World::add(new Flash(orb->pos + Vector2f(0, 3)));

				} else if (CollisionBox::check(World::players->at(j)->bodyBox, orb->box)) {
					//std::cout << "Auuu!\n";
				}
			}

			if ((e = dynamic_cast<Enemy *>(World::objects->at(i))) != NULL) {
				if (CollisionBox::check(World::players->at(j)->swordBox, e->box)) {

					e->kill();
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
    //gamePixelArea.clear();

    monitorPixelArea.clear(Color(0, 0, 0, 0));
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
		float scale_multiply = size(smallest_most - largest_most) + 600.0f;

		// 600
		if (scale_multiply < 600.0f) {
			scale_multiply = 600.0f;
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
			sorted.at(movable_index)->draw(target, &monitorPixelArea);
			movable_index += 1;
		}
		map->images[i].draw(target);
	}

    for (unsigned int i = movable_index; i < sorted.size(); i++) {
		sorted.at(i)->draw(target, &monitorPixelArea);
	}


	// only for debug
	map->drawDebug(target);


    /*gamePixelArea.display();
    monitorPixelArea.display();
    
    {
        Sprite sprite(gamePixelArea.getTexture());
        sprite.setOrigin(gamePixelArea.getSize().x / 2, gamePixelArea.getSize().y / 2);
        sprite.setPosition((int)(targetSize.x / 2.f), (int)(targetSize.y / 2.f));
        sprite.setScale(scaleFactor,scaleFactor);
        target->draw(sprite);
    }
    
    {
        Sprite sprite(monitorPixelArea.getTexture());
        sprite.setOrigin(gamePixelArea.getSize().x / 2 * scaleFactor, gamePixelArea.getSize().y / 2 * scaleFactor);
        sprite.setPosition((int)(targetSize.x / 2.f), (int)(targetSize.y / 2.f));
        target->draw(sprite);
    }*/
}

