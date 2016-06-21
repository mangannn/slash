#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <iostream>

using namespace sf;
using namespace std;

#include "Game.hpp"

#include "InputCodeString.hpp"

#define WINDOW_TITLE ("FLUX!")
#define FRAMERATE_LIMIT (50)

sf::Texture startScreenTex;
sf::Vector2i startScreenTexSize(1900, 1500);

sf::Texture standingTex, runningTex;
sf::Texture bodyTex, eyesTex, mouthTex;
sf::Texture bollTex;

sf::Vector2i playerSpriteSize(200, 300);

sf::Vector2i bodySize(745, 745);
sf::Vector2i eyesSize(455, 35);
sf::Vector2i mouthSize(130, 50);

sf::Vector2i bollSize(125, 125);

sf::Font font;


bool load_resources() {

	std::cout << "Loading textures..." << std::endl;
	/*if (
		!startScreenTex.loadFromFile("media/images/FLUX.png") ||
		!bollTex.loadFromFile("media/images/boll/strip.png")) {
		
		return false;
	}*/
	std::cout << "Done!" << std::endl;
/*
	standingTex.setSmooth(true);
	runningTex.setSmooth(true);
	bodyTex.setSmooth(true);
	eyesTex.setSmooth(true);
	mouthTex.setSmooth(true);
	bollTex.setSmooth(true);
*/


	std::cout << "Loading font..." << std::endl;
	if (!font.loadFromFile("media/fonts/8bitlimo.ttf")) {
	    return false;
	}
	std::cout << "Done!" << std::endl;

	return true;
}

int main() {

	if (!load_resources()) {
		return 1;
	}


	srand(time(NULL));

	sf::ContextSettings settings;
	settings.antialiasingLevel = 0;

	sf::RenderWindow *window;
	bool fullscreen = false;

	window = new RenderWindow(VideoMode(800, 620), WINDOW_TITLE, sf::Style::Resize | sf::Style::Close, settings);
	window->setMouseCursorVisible(false);
	window->setKeyRepeatEnabled(false);
	window->setVerticalSyncEnabled(false);
	window->setFramerateLimit(FRAMERATE_LIMIT);



	Game *game = new Game();



	sf::Event event;
	sf::Clock clock;

	while (window->isOpen()) {

		float elapsedTime = clock.restart().asSeconds();

		while (window->pollEvent(event)) {

			game->eventHandle(event);

			switch (event.type) {
				case sf::Event::Closed: {
					window->close();
				} break;
				case sf::Event::KeyPressed: {

					switch (event.key.code) {
						case sf::Keyboard::Escape: {
							window->close();
						} break;

						case sf::Keyboard::F2:
						case sf::Keyboard::F3:
						case sf::Keyboard::F4:
						case sf::Keyboard::F5:
						case sf::Keyboard::F11:
						{

							fullscreen = !fullscreen;

							if (fullscreen) {
								window->create(sf::VideoMode::getFullscreenModes().front(), WINDOW_TITLE, sf::Style::Fullscreen | sf::Style::Close, settings);
							} else {
								window->create(VideoMode(800, 620), WINDOW_TITLE, sf::Style::Resize | sf::Style::Close, settings);
							}

							window->setMouseCursorVisible(false);
							window->setKeyRepeatEnabled(false);
							window->setVerticalSyncEnabled(false);
							window->setFramerateLimit(FRAMERATE_LIMIT);

						} break;
						default: break;
					}
				} break;

				case sf::Event::JoystickConnected: {

					int id = event.joystickConnect.joystickId;

					std::cout << "Joystick connected:\t" << id << std::endl;
					std::cout << "Button Count:\t" << sf::Joystick::getButtonCount(id) << std::endl;

					std::cout << "Axises:\t";
					for (int i = 0; i < sf::Joystick::AxisCount; i++) {
						if (sf::Joystick::hasAxis(id, (sf::Joystick::Axis)i)) {
							std::cout << joystickAxisToString(i) << " ";
						}
					}
					std::cout << std::endl;
				} break;
				case sf::Event::JoystickDisconnected: {
					std::cout << "Joystick disconnected:\t" << event.joystickConnect.joystickId << std::endl;
				} break;
				default: break;
			}
		}

		game->update(elapsedTime);

		game->draw(window);

		//Update window
		window->display();
	}

	delete game;

	delete window;

	return 0;
}
