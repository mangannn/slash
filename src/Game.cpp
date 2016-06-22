#include <math.h>
#include <iostream>

#include "Resources.hpp"
#include "Functions.hpp"

#include "Game.hpp"

#include "IvansTestAni/head.hpp"
#include "IvansTestAni/titelEffekt.hpp"


Game::Game() {
    
    scaleFactor = 3.0;
    
    gamePixelArea.create(320, 240);
    monitorPixelArea.create((int)(320 * scaleFactor), (int)(240 * scaleFactor));
    
    //gamePixelArea.setSmooth(false);
    
    setMainFont("media/fonts/pixlfont.ttf");
    
	mapTex.loadFromFile("media/images/map.png");
	mapSprite.setTexture(mapTex);
	mapSprite.setOrigin(sf::Vector2f((float)mapTex.getSize().x / 2.0f, (float)mapTex.getSize().y / 2.0f));
	mapSprite.setScale(10.0f, 10.0f);


	players = new std::vector<Player *>();

	players->push_back(new Player(Vector2f(0,0)));

	orbs = new std::vector<Orb *>();


	objects = new std::vector<Object *>();

    
    objects->push_back(new RotAni(Vector2f(0,-10)));
    objects->push_back(new TitelEffekt(Vector2f(120,100)));

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
		orbs->push_back(new Orb(Vector2f(0,0), 100.0f * Vector2f(RANDOM2, RANDOM2)));
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
    monitorPixelArea.clear(Color(0, 0, 0, 0));
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
		// gameView.setSize(newSize);
		gameView.setCenter((newPosition - currentPosition) / 4.0f + currentPosition);
		
		gamePixelArea.setView(gameView);

	}

	gamePixelArea.draw(mapSprite);


	for (unsigned int i = 0; i < orbs->size(); i++) {
		orbs->at(i)->draw(&gamePixelArea, &monitorPixelArea);
	}
	for (unsigned int i = 0; i < players->size(); i++) {
		players->at(i)->draw(&gamePixelArea, &monitorPixelArea);
	}
	for (unsigned int i = 0; i < objects->size(); i++) {
		objects->at(i)->draw(&gamePixelArea, &monitorPixelArea);
	}
    
    gamePixelArea.display();
    monitorPixelArea.display();
    
    {
        Sprite sprite(gamePixelArea.getTexture());
        sprite.setOrigin(160, 120);
        sprite.setPosition((int)(windowSize.x / 2.f), (int)(windowSize.y / 2.f));
        sprite.setScale(scaleFactor,scaleFactor);
        window->draw(sprite);
    }
    
    {
        Sprite sprite(monitorPixelArea.getTexture());
        sprite.setOrigin(320 / 2 * scaleFactor, 240 / 2 * scaleFactor);
        sprite.setPosition((int)(windowSize.x / 2.f), (int)(windowSize.y / 2.f));
        window->draw(sprite);
    }
}
