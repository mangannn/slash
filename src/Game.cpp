#include <math.h>
#include <iostream>

#include "Functions.hpp"

#include "Game.hpp"

#include "obj/Rock.hpp"

#include "IvansTestAni/head.hpp"

Game::Game() {

	mapTex.loadFromFile("media/images/map.png");
	mapSprite.setTexture(mapTex);
	mapSprite.setOrigin(sf::Vector2f((float)mapTex.getSize().x / 2.0f, (float)mapTex.getSize().y / 2.0f));
	mapSprite.setScale(10.0f, 10.0f);


	players = new std::vector<Player *>();

	players->push_back(new Player(Vector2f(0,0)));

	orbs = new std::vector<Orb *>();


	objects = new std::vector<Object *>();



	objects->push_back(new Rock(Vector2f(0,0)));
    objects->push_back(new RotAni(Vector2f(0,-10)));

	gameView.setSize(Vector2f(1000, 1000));
	gameView.setCenter(Vector2f(0,0));;
}
Game::~Game() {

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

	for (unsigned int i = 0; i < players->size(); i++) {
		players->at(i)->controls->eventHandle(event);
	}

	switch (event.type) {
		case sf::Event::KeyPressed: {

			switch (event.key.code) {
				case sf::Keyboard::Return: {
					std::cout << orbs->size() << std::endl;
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
		orbs->push_back(new Orb(Vector2f(0,0), 70.0f * Vector2f(RANDOM2, RANDOM2)));
	}


	for (unsigned int i = 0; i < orbs->size(); i++) {
		if (size(orbs->at(i)->pos - gameView.getCenter()) > 400) {
			Orb *o = orbs->at(i);
			orbs->erase(orbs->begin() + i);
			delete o;
			i -= 1;
		}
	}


	for (unsigned int j = 0; j < players->size(); j++) {
		for (unsigned int i = 0; i < orbs->size(); i++) {
			
		}
	}
}





void Game::draw(RenderWindow *window) {

	window->clear();

	float aspect = ((float)window->getSize().x / (float)window->getSize().y);

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
		
		window->setView(gameView);

	}



	window->draw(mapSprite);


	for (unsigned int i = 0; i < orbs->size(); i++) {
		orbs->at(i)->draw(window);
	}
	for (unsigned int i = 0; i < players->size(); i++) {
		players->at(i)->draw(window);
	}
	for (unsigned int i = 0; i < objects->size(); i++) {
		objects->at(i)->draw(window);
	}

}
