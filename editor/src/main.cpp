#include <SFML/Graphics.hpp>

#include <iostream>

#include "Editor.hpp"

#define WINDOW_TITLE ("Editor")
#define FRAMERATE_LIMIT (60)

int main() {

	srand(time(NULL));

	sf::ContextSettings settings;
	settings.antialiasingLevel = 0;

	sf::RenderWindow *window;
	bool fullscreen = false;
    
	window = new RenderWindow(sf::VideoMode(800, 620), WINDOW_TITLE, sf::Style::Resize | sf::Style::Close, settings);
	//window = new RenderWindow(sf::VideoMode::getFullscreenModes()[0], WINDOW_TITLE, sf::Style::Fullscreen, settings);
	window->setMouseCursorVisible(true);
	window->setKeyRepeatEnabled(false);
	window->setVerticalSyncEnabled(false);
	window->setFramerateLimit(FRAMERATE_LIMIT);

	
    setMainFont("media/fonts/pixlfont.ttf");


	Editor *editor = new Editor(window);


	sf::Event event;

	while (window->isOpen()) {

		while (window->pollEvent(event)) {

			editor->eventHandle(event);

			switch (event.type) {
				case sf::Event::Closed: {
					window->close();
				} break;
				case sf::Event::KeyPressed: {

					switch (event.key.code) {
						case sf::Keyboard::Escape: {
							window->close();
						} break;

						case sf::Keyboard::F11:
						{

							fullscreen = !fullscreen;

							if (fullscreen) {
								window->create(sf::VideoMode::getFullscreenModes().front(), WINDOW_TITLE, sf::Style::Fullscreen | sf::Style::Close, settings);
							} else {
								window->create(VideoMode(800, 620), WINDOW_TITLE, sf::Style::Resize | sf::Style::Close, settings);
							}

							window->setMouseCursorVisible(true);
							window->setKeyRepeatEnabled(false);
							window->setVerticalSyncEnabled(false);
							window->setFramerateLimit(FRAMERATE_LIMIT);

						} break;
						default: break;
					}
				} break;
				default: break;
			}
		}

		editor->draw();

		//Update window
		window->display();
	}

	delete editor;

	delete window;

	return 0;
}
