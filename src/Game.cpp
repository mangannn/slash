#include <math.h>
#include <iostream>

#include "Functions.hpp"

#include "Game.hpp"

#include "IvansTestAni/head.hpp"

#include "Collision.hpp"

Game::Game() {
    
    gamePixelArea.create(320, 240);
    gamePixelArea.setSmooth(false);


	players = new std::vector<Player *>();

	players->push_back(new Player(Vector2f(0,0)));

	orbs = new std::vector<Orb *>();


	objects = new std::vector<Object *>();

	map = new Map();


    objects->push_back(new RotAni(Vector2f(0,-10)));

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
		orbs->push_back(new Orb(Vector2f(0,0), 100.0f * Vector2f(RANDOM2, RANDOM2)));
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
			Vector2f swS = players->at(j)->pos;
			Vector2f swE = swS + players->at(j)->swordDir * players->at(j)->swordLen;

			Vector2f v = swE - swS;
			Vector2f u = orbs->at(i)->pos - swS;
			Vector2f b = orbs->at(i)->vel;

			if (size(u - (dot(v, u) / sqrSize(v)) * v) < orbs->at(i)->radius) {
				orbs->at(i)->vel = b - 2 * (dot(v, b) / sqrSize(v)) * v;
			}
		}
	}
}




void Game::draw(RenderTarget *window) {

	window->clear();
    gamePixelArea.clear();
    Vector2u windowSize = window->getSize();
	float aspect = ((float)windowSize.x / (float)windowSize.y);

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

		if (scale_multiply < 240.0f) {
			scale_multiply = 240.0f;
		}

		Vector2f newPosition = (smallest_most + largest_most) / 2.0f;
		Vector2f newSize = Vector2f(aspect, 1.0f) * scale_multiply;

		Vector2f currentSize = gameView.getSize();
		Vector2f currentPosition = gameView.getCenter();

		gameView.setSize((newSize - currentSize) / 4.0f + currentSize);
		gameView.setCenter((newPosition - currentPosition) / 4.0f + currentPosition);
		
		gamePixelArea.setView(gameView);

	}

	map->draw(&gamePixelArea);


	for (unsigned int i = 0; i < orbs->size(); i++) {
		orbs->at(i)->draw(&gamePixelArea);
	}
	for (unsigned int i = 0; i < players->size(); i++) {
		players->at(i)->draw(&gamePixelArea);
	}
	for (unsigned int i = 0; i < objects->size(); i++) {
		objects->at(i)->draw(&gamePixelArea);
	}
    
    gamePixelArea.display();
    
    Sprite sprite(gamePixelArea.getTexture());
    sprite.setOrigin(gamePixelArea.getSize().x / 2.f, gamePixelArea.getSize().y / 2.f);
    sprite.setPosition(windowSize.x / 2.f, windowSize.y / 2.f);
    sprite.setScale(3,3);
    window->draw(sprite);
}
