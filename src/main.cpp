#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <iostream>

#include "Functions.hpp"
#include "Resources.hpp"

#include "Game.hpp"

#define WINDOW_TITLE ("Slash")
#define FRAMERATE_LIMIT (60)

int main() {

	srand(time(NULL));

	sf::ContextSettings settings;
	settings.antialiasingLevel = 0;

	sf::RenderWindow *window;
	bool fullscreen = false;
    
	window = new RenderWindow(sf::VideoMode(800, 620), WINDOW_TITLE, sf::Style::Resize | sf::Style::Close, settings);
	//window = new RenderWindow(sf::VideoMode::getFullscreenModes()[0], WINDOW_TITLE, sf::Style::Fullscreen, settings);
	window->setMouseCursorVisible(false);
	window->setKeyRepeatEnabled(false);
	window->setVerticalSyncEnabled(false);
	window->setFramerateLimit(FRAMERATE_LIMIT);


    
    setMainFont("media/fonts/pixlfont.ttf");


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
					std::cout << "Joystick connected:\t" << event.joystickConnect.joystickId << std::endl;
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
