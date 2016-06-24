#include <math.h>
#include <iostream>

#include "Resources.hpp"
#include "Functions.hpp"

#include "Game.hpp"

#include "IvansTestAni/head.hpp"
#include "IvansTestAni/titelEffekt.hpp"


#include "obj/Enemy.hpp"
#include "obj/Explotion.hpp"


#include "Collision.hpp"

Game::Game() {
    
    scaleFactor = 3.0;
    
    gamePixelArea.create(320, 240);
    monitorPixelArea.create((int)(gamePixelArea.getSize().x * scaleFactor), (int)(gamePixelArea.getSize().y * scaleFactor));
    
    //gamePixelArea.setSmooth(false);
    


	players = new std::vector<Player *>();

	players->push_back(new Player(Vector2f(0,0), 0));
	//players->push_back(new Player(Vector2f(50,0), 1));

	orbs = new std::vector<Orb *>();


	objects = new std::vector<Object *>();


	map = new Map();

    objects->push_back(new RotAni(Vector2f(0,-10)));
    objects->push_back(new TitelEffekt(Vector2f(120,100)));

	gameView.setSize(Vector2f(1000, 1000));
	gameView.setCenter(Vector2f(0,0));;
}
Game::~Game() {

	delete map;

	{
		Object *temp;
		while (!objects->empty()) {
			temp = objects->back();
			delete temp;
			objects->pop_back();
		}
		delete objects;
	}
	{
		Object *temp;
		while (!orbs->empty()) {
			temp = orbs->back();
			delete temp;
			orbs->pop_back();
		}
		delete orbs;
	}
	{
		Object *temp;
		while (!players->empty()) {
			temp = players->back();
			delete temp;
			players->pop_back();
		}
		delete players;
	}
}



void Game::eventHandle(sf::Event event) {

	switch (event.type) {
		case sf::Event::KeyPressed: {

			switch (event.key.code) {
				case sf::Keyboard::Return: {
					std::cout << orbs->size() << std::endl;
				} break;
				case sf::Keyboard::A: {
					std::cout << players->at(0)->pos.x << " :\t" << players->at(0)->pos.y << std::endl;
				} break;
				default: break;
			}
		} break;
		default: break;
	}
}







void Game::update(float elapsedTime) {

	for (unsigned int i = 0; i < objects->size(); i++) {
		objects->at(i)->update(elapsedTime);
	}
	for (unsigned int i = 0; i < orbs->size(); i++) {
		orbs->at(i)->update(elapsedTime);
	}
	for (unsigned int i = 0; i < players->size(); i++) {
		players->at(i)->update(elapsedTime);
	}

	orbTimer += elapsedTime;

	if (orbTimer > 2) {
		orbTimer -= 2;
		orbs->push_back(new Orb(Vector2f(0,0), 40.0f * Vector2f(RANDOM2, RANDOM2)));
    	objects->push_back(new Enemy(Vector2f(300, 300)));
	}


	for (unsigned int i = 0; i < orbs->size(); i++) {
		if (sqrSize(orbs->at(i)->pos - gameView.getCenter()) > 400 * 400) {
			Orb *o = orbs->at(i);
			orbs->erase(orbs->begin() + i);
			delete o;
			i -= 1;
		}
	}


	for (unsigned int j = 0; j < players->size(); j++) {
		for (unsigned int i = 0; i < map->numStatic; i++) {
			if (CollisionBox::check(players->at(j)->walkBox, map->staticBoxes[i])) {
				float coll_dist = players->at(j)->walkBox.r + map->staticBoxes[i].r;
				Vector2f diff = players->at(j)->walkBox.getPosition() - map->staticBoxes[i].getPosition();
				players->at(j)->pos += diff * (coll_dist / size(diff) - 1);
			}
		}
	}

	for (unsigned int j = 0; j < players->size(); j++) {

		for (unsigned int i = 0; i < orbs->size(); i++) {
			if (CollisionBox::check(players->at(j)->swordBox, orbs->at(i)->box)) {
				Vector2f diff = players->at(j)->swordBox.getPosition() - orbs->at(i)->pos;
				orbs->at(i)->vel = orbs->at(i)->vel - 2 * (dot(orbs->at(i)->vel, diff) / sqrSize(diff)) * diff;
			} else if (CollisionBox::check(players->at(j)->bodyBox, orbs->at(i)->box)) {
				//std::cout << "Auuu!\n";
			}
		}

		Enemy *e;

		for (unsigned int i = 0; i < objects->size(); i++) {
			if ((e = dynamic_cast<Enemy *>(objects->at(i))) != NULL) {
				if (CollisionBox::check(players->at(j)->swordBox, e->box)) {

    				objects->push_back(new Explotion(e->pos + Vector2f(0, 20)));

					objects->erase(objects->begin() + i);
					delete e;
					i -= 1;

				}
			}
		}
	}
}



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
    gamePixelArea.clear();

    monitorPixelArea.clear(Color(0, 0, 0, 0));
    Vector2u targetSize = target->getSize();
	//float aspect = ((float)targetSize.x / (float)targetSize.y);

	// set game view
	{

		std::vector<Player *> *followedObjects = players;

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


		float scale_multiply = size(smallest_most - largest_most) + 100.0f;

		if (scale_multiply < 300.0f) {
			scale_multiply = 300.0f;
		}

		Vector2f newPosition = (smallest_most + largest_most) / 2.0f;
		Vector2f currentPosition = gameView.getCenter();

		gameView.setCenter((newPosition - currentPosition) / 4.0f + currentPosition);

		float newMultiply = scale_multiply;
		float currentMultiply = gameView.getSize().y;

		float aspect = ((float)gamePixelArea.getSize().x / (float)gamePixelArea.getSize().y);

		gameView.setSize(Vector2f(aspect, 1.0f) * ((newMultiply - currentMultiply) / 4.0f + currentMultiply));
		
		gamePixelArea.setView(gameView);

	}

	map->drawBackground(&gamePixelArea);


	std::vector<Object *> sorted; // sorted by depth


	for (unsigned int i = 0; i < orbs->size(); i++) {
		insertByDepth(&sorted, orbs->at(i));
	}
	for (unsigned int i = 0; i < players->size(); i++) {
		insertByDepth(&sorted, players->at(i));
	}
	for (unsigned int i = 0; i < objects->size(); i++) {
		insertByDepth(&sorted, objects->at(i));
	}


	unsigned int movable_index = 0;

	for (unsigned int i = 0; i < map->numImages; i++) {
		while (movable_index < sorted.size() && sorted.at(movable_index)->pos.y < map->images[i].pos.y) {
			sorted.at(movable_index)->draw(&gamePixelArea, &monitorPixelArea);
			movable_index += 1;
		}
		map->images[i].draw(&gamePixelArea);
	}

    for (unsigned int i = movable_index; i < sorted.size(); i++) {
		sorted.at(i)->draw(&gamePixelArea, &monitorPixelArea);
	}


	// only for debug
	map->drawDebug(&gamePixelArea);


    gamePixelArea.display();
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
    }
}

