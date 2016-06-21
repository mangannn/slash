#include <math.h>

#include "Functions.hpp"

#include "Game.hpp"

#include "obj/Rock.hpp"


Game::Game() {

	mapTex.loadFromFile("media/images/map.png");
	mapSprite.setTexture(mapTex);


	players = new std::vector<Player *>();


	int a[] = {13, 12};
	players->push_back(new Player(Vector2f(0,0), new KeyboardControls(73, 74, 71, 72, a, 2)));
	//int a[] = {0, 1};
	//players->push_back(new Player(Vector2f(0,0), new JoystickControls(0, 0, 1, a, 2)));


	objects = new std::vector<Object *>();

	for (unsigned int i = 0; i < players->size(); i++) {
		objects->push_back(players->at(i));
	}



	objects->push_back(new Rock(Vector2f(0,0)));


	gameView.setSize(Vector2f(1000, 1000));
	gameView.setCenter(Vector2f(0,0));;
}
Game::~Game() {

	// dont delete the objects i the player vector because they are also a part och the objects vector
	delete players;

	{
		Object *temp;
		while (!objects->empty()) {
			temp = objects->back();
			delete temp;
			objects->pop_back();
		}
		delete objects;
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
					printf("enter\n");
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

		smallest_most.x /= aspect;
		largest_most.x /= aspect;


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


	for (unsigned int i = 0; i < objects->size(); i++) {
		objects->at(i)->draw(window);
	}

}
