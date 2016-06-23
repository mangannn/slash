#ifndef _Editor_H_
#define _Editor_H_

#include <string.h>

#include <iostream>

#include <SFML/Graphics.hpp>

#include "obj/Image.hpp"
#include "Collision.hpp"

class Editor {

public:

	std::string mapFilename = "media/map.txt";

	sf::View mapView, guiView;

	sf::Texture mapTex;
	sf::Sprite mapSprite;

	std::vector<cs::Image> images;
	std::vector<CollisionBox> staticBoxes;


	Vector2f *origo = new Vector2f(0,0);


	bool mouseMove = false;
	Vector2f mouseMoveVec;

	int selected = 0;
	sf::RectangleShape selectedRect;

	int state = 0;

	Editor() {
		loadMap(mapFilename.c_str());
    
		mapTex.loadFromFile("media/images/map.png");
		mapSprite.setTexture(mapTex);
		mapSprite.setOrigin(sf::Vector2f((float)mapTex.getSize().x / 2.0f, (float)mapTex.getSize().y / 2.0f));
		mapSprite.setScale(8.0f, 8.0f);

		selectedRect.setSize(sf::Vector2f(40, 40));
		selectedRect.setOrigin(sf::Vector2f((float)selectedRect.getSize().x / 2.0f, (float)selectedRect.getSize().y / 2.0f));
		selectedRect.setFillColor(sf::Color(0, 0, 0, 0));
		selectedRect.setOutlineThickness(5);
		selectedRect.setOutlineColor(sf::Color(255, 0, 0));
	}

	~Editor() {
		saveMap("media/autosave.txt");
	}


	void eventHandle(sf::Event event, sf::RenderTarget *target) {
		switch (event.type) {
			case sf::Event::MouseMoved: {
				if (mouseMove) {

					Vector2f coord = target->mapPixelToCoords(Vector2i(event.mouseMove.x, event.mouseMove.y));

					mapView.move(mouseMoveVec - coord);
				}
			} break;
			case sf::Event::MouseButtonPressed: {
				switch (event.mouseButton.button) {
					case sf::Mouse::Left: {

						Vector2f coord = target->mapPixelToCoords(Vector2i(event.mouseButton.x, event.mouseButton.y));

						if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
							if (state == 0) {
								staticBoxes.at(selected).pos = coord;
							} else {
								images.at(selected).pos = coord;
							}
						} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
							if (state == 0) {
								staticBoxes.push_back(CollisionBox(origo, coord, 100));
								selected = staticBoxes.size() - 1;
							} else {
								images.push_back(cs::Image(coord, "media/images/gran.png"));
								selected = images.size() - 1;
							}
						} else {
							if (state == 0) {

								int s = 0;
								float dist = size(staticBoxes.at(s).pos - coord);

								for (unsigned int i = 1; i < staticBoxes.size(); i++) {
									float d = size(staticBoxes.at(i).pos - coord);
									if (d < dist) {
										s = i;
										dist = d;
									}
								}

								selected = s;

							} else {
								int s = 0;
								float dist = size(images.at(s).pos - coord);

								for (unsigned int i = 1; i < images.size(); i++) {
									float d = size(images.at(i).pos - coord);
									if (d < dist) {
										s = i;
										dist = d;
									}
								}

								selected = s;
							}
						}
					} break;
					case sf::Mouse::Right: {
						mouseMoveVec = target->mapPixelToCoords(Vector2i(event.mouseButton.x, event.mouseButton.y));
						mouseMove = true;

					} break;
					default: break;
				}
			} break;
			case sf::Event::MouseButtonReleased: {
				switch (event.mouseButton.button) {
					case sf::Mouse::Right: {
						mouseMove = false;
					} break;
					default: break;
				}
			} break;
			case sf::Event::MouseWheelMoved: {
				mapView.zoom(event.mouseWheel.delta * 0.1f + 1.0f);
			} break;			
			case sf::Event::KeyPressed: {

				switch (event.key.code) {
					case sf::Keyboard::Return: {
					} break;
					case sf::Keyboard::S: {
						saveMap(mapFilename.c_str());
					} break;
					case sf::Keyboard::Num1: {
						state = 0;
						selected = 0;
					} break;
					case sf::Keyboard::Num2: {
						state = 1;
						selected = 0;
					} break;
					default: break;
				}
			} break;
			default: break;
		}
	}

	void draw(sf::RenderTarget *target) {

		target->clear();

	    Vector2u targetSize = target->getSize();
		float aspect = ((float)targetSize.x / (float)targetSize.y);

		mapView.setSize(mapView.getSize().y * aspect, mapView.getSize().y);
		guiView.setSize(guiView.getSize().y * aspect, guiView.getSize().y);

		target->setView(mapView);

		target->draw(mapSprite);

		for (unsigned int i = 0; i < images.size(); i++) {
			images.at(i).draw(target);
		}
		for (unsigned int i = 0; i < staticBoxes.size(); i++) {
			staticBoxes.at(i).draw(target);
		}

		Vector2i pixel;
		if (state == 0) {
			pixel = target->mapCoordsToPixel(staticBoxes.at(selected).pos);
		} else {
			pixel = target->mapCoordsToPixel(images.at(selected).pos);
		}

		target->setView(guiView);

		selectedRect.setPosition(target->mapPixelToCoords(pixel));

		target->draw(selectedRect);



		target->setView(mapView);
	}

	void loadMap(const char* path) {

		FILE *file;

		if (!(file = fopen(path, "r"))) {
			std::cout << "Failed to open file: " << path << std::endl;
			exit(-1);
		}

		const int MAXSTR = 256;
		char buffer[MAXSTR];

		char str1[MAXSTR];
		char str2[MAXSTR];

		float x = 0, y = 0, r = 0;

		int num_back;

		while (fgets(buffer, MAXSTR, file)) {

			num_back = sscanf(buffer, 
				"%[^{\n]{%[^}\n]}", 
				str1, str2);

			if (num_back != 2) {
				continue;
			}

			if (strcmp(str1, "box") == 0) {

				num_back = sscanf(str2, 
					"%f,%f,%f", 
					&x, &y, &r);

				if (num_back != 3) {
					continue;
				}

				staticBoxes.push_back(CollisionBox(origo, Vector2f(x, y), r));

			} else if (strcmp(str1, "img") == 0) {

				num_back = sscanf(str2, 
					"%[^,\n],%f,%f", 
					str1, &x, &y);

				if (num_back != 3) {
					continue;
				}


				images.push_back(cs::Image(Vector2f(x, y), str1));
			}
		}
		fclose(file);
	}

	void saveMap(const char* path) {

		FILE *file;

		if (!(file = fopen(path, "w"))) {
			std::cout << "Failed to open file: " << path << std::endl;
			exit(-1);
		}

		for (unsigned int i = 0; i < staticBoxes.size(); i++) {

			if (fprintf(file, 
				"box{%f, %f, %f}\n", 
				staticBoxes.at(i).pos.x, staticBoxes.at(i).pos.y, staticBoxes.at(i).r) < 1) {

				std::cout << "Error writeing to file: " << path << std::endl;
				exit(-1);
			}
		}
		for (unsigned int i = 0; i < images.size(); i++) {

			if (fprintf(file, 
				"img{%s, %f, %f}\n", 
				"#######", images.at(i).pos.x, images.at(i).pos.y) < 1) {

				std::cout << "Error writeing to file: " << path << std::endl;
				exit(-1);
			}
		}
		fclose(file);
	}
};

#endif
