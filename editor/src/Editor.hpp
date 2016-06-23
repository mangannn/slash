#ifndef _Editor_H_
#define _Editor_H_

#include <string.h>

#include <iostream>
#include <iomanip>

#include <SFML/Graphics.hpp>

#include "Collision.hpp"


#include "Resources.hpp"


class ImageM {

public:

	Vector2f pos;

	sf::Sprite sprite;

	std::string filename;

	ImageM(Vector2f position, std::string filenameParam):
		pos(position),
		filename(filenameParam)
	{

		sf::Texture *texture = getTexture(filename);
		sprite.setTexture(*texture);
		sprite.setOrigin(sf::Vector2f((float)texture->getSize().x / 2.0f, (float)texture->getSize().y));
		//pos.y += (float)texture->getSize().y / 2.0f;
	}

	virtual ~ImageM() {}

	virtual void draw(RenderTarget *target) {
		sprite.setPosition(pos);
		target->draw(sprite);
	}
};


class Editor {

public:

	std::string mapFilename = "media/maps/waterfall.txt";

	sf::View mapView, guiView;

	sf::Texture mapTex;
	sf::Sprite mapSprite;

	std::vector<ImageM> images;
	std::vector<CollisionBox> staticBoxes;


	Vector2f *origo = new Vector2f(0,0);


	bool mouseMove = false;
	Vector2f mouseMoveVec;

	int selected = 0;
	sf::RectangleShape selectedRect;

	int state = 0;


	sf::Text infoText, helpText;




	sf::RenderWindow *target;

	Editor(sf::RenderWindow *targetParam): 
		target(targetParam)
	{
		loadMap(mapFilename.c_str());
    
		mapTex.loadFromFile("media/maps/waterfall.png");
		mapSprite.setTexture(mapTex);
		mapSprite.setOrigin(sf::Vector2f((float)mapTex.getSize().x / 2.0f, (float)mapTex.getSize().y / 2.0f));

		selectedRect.setSize(sf::Vector2f(30, 30));
		selectedRect.setOrigin(sf::Vector2f((float)selectedRect.getSize().x / 2.0f, (float)selectedRect.getSize().y / 2.0f));
		selectedRect.setFillColor(sf::Color(0, 0, 0, 0));
		selectedRect.setOutlineThickness(5);
		selectedRect.setOutlineColor(sf::Color(255, 0, 0));


        infoText.setFont(*getMainFont());
        infoText.setCharacterSize(40);
        infoText.setPosition(20, 0);

        helpText.setFont(*getMainFont());
        helpText.setCharacterSize(40);
        helpText.setPosition(300, 0);

        helpText.setString(sf::String(
        	"h:\tthis help text\ns:\tsave\n1:\tstate 1\n2:\tstate 2\nz:\tdelete selected object\n\nMouse:\n\n") + sf::String(
        	"right:     move screen\nleft:      select object\nleft+ctrl:  move selected object\n") + sf::String(
        	"left+shift: copy selected object\nwheel:     zoom\nwheel+alt:   change size (collision)"
        	));
	}

	~Editor() {
		saveMap("media/maps/autosave.txt");
	}


	void eventHandle(sf::Event event) {
		switch (event.type) {
			case sf::Event::MouseMoved: {
				if (mouseMove) {

					Vector2f coord = target->mapPixelToCoords(Vector2i(event.mouseMove.x, event.mouseMove.y), mapView);

					mapView.move(mouseMoveVec - coord);
				}
			} break;
			case sf::Event::MouseButtonPressed: {
				switch (event.mouseButton.button) {
					case sf::Mouse::Left: {

						Vector2f coord = target->mapPixelToCoords(Vector2i(event.mouseButton.x, event.mouseButton.y), mapView);

						if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
							if (state == 0) {
								staticBoxes.at(selected).pos = coord;
							} else {
								images.at(selected).pos = coord;
							}
						} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
							if (state == 0) {
								staticBoxes.push_back(CollisionBox(origo, coord, staticBoxes.at(selected).r));
								selected = staticBoxes.size() - 1;
							} else {
								selected = addImage(ImageM(coord, images.at(selected).filename));
							}
						} else {
							selectClosest(coord);
						}
					} break;
					case sf::Mouse::Right: {
						mouseMoveVec = target->mapPixelToCoords(Vector2i(event.mouseButton.x, event.mouseButton.y), mapView);
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
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt)) {
					if (state == 0) {
						staticBoxes.at(selected).r *= 1.f + (0.1f * event.mouseWheel.delta);
					} else {

					}
				} else {
                    
                    sf::Vector2f mousePosBefore = target->mapPixelToCoords(sf::Mouse::getPosition(*target), mapView);
					mapView.zoom(1.f + (0.1f * event.mouseWheel.delta));
                    mapView.move(mousePosBefore - target->mapPixelToCoords(sf::Mouse::getPosition(*target), mapView));
                    
				}
			} break;			
			case sf::Event::KeyPressed: {

				switch (event.key.code) {
					case sf::Keyboard::Z: {
						if (state == 0) {
							if (staticBoxes.size() > 1) {
								staticBoxes.erase(staticBoxes.begin() + selected);
								selectClosest(target->mapPixelToCoords(sf::Mouse::getPosition(*target), mapView));
							}
						} else {
							if (images.size() > 1) {
								images.erase(images.begin() + selected);
								selectClosest(target->mapPixelToCoords(sf::Mouse::getPosition(*target), mapView));
							}
						}
					} break;
					case sf::Keyboard::S: {
						saveMap(mapFilename.c_str());
						std::cout << "Saved!\n";
					} break;
					case sf::Keyboard::Num1: {
						state = 0;
						selectClosest(target->mapPixelToCoords(sf::Mouse::getPosition(*target), mapView));
					} break;
					case sf::Keyboard::Num2: {
						state = 1;
						selectClosest(target->mapPixelToCoords(sf::Mouse::getPosition(*target), mapView));
					} break;
					default: break;
				}
			} break;
			default: break;
		}
	}

	void draw() {

		target->clear();

	    Vector2u targetSize = target->getSize();
		float aspect = ((float)targetSize.x / (float)targetSize.y);

		mapView.setSize(mapView.getSize().y * aspect, mapView.getSize().y);
		guiView.setSize(target->getSize().y * aspect, target->getSize().y);
		guiView.setCenter(guiView.getSize() * 0.5f);

		target->setView(mapView);

		target->draw(mapSprite);

		for (unsigned int i = 0; i < images.size(); i++) {
			images.at(i).draw(target);
		}
		for (unsigned int i = 0; i < staticBoxes.size(); i++) {
			staticBoxes.at(i).draw(target);
		}



		target->setView(guiView);



		Vector2i pixel;
		if (state == 0) {
			pixel = target->mapCoordsToPixel(staticBoxes.at(selected).pos, mapView);
		} else {
			pixel = target->mapCoordsToPixel(images.at(selected).pos, mapView);
		}

		selectedRect.setPosition(target->mapPixelToCoords(pixel, guiView));

		target->draw(selectedRect);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::H)) {
			target->draw(helpText);
		}


		updateText();
		target->draw(infoText);
	}






	void selectClosest(Vector2f coord) {

		if (state == 0) {

			int s = 0;
			float dist = 100000000.0f;

			for (unsigned int i = 0; i < staticBoxes.size(); i++) {
				float d = size(staticBoxes.at(i).pos - coord);
				if (d < dist) {
					s = i;
					dist = d;
				}
			}

			selected = s;

		} else {
			int s = 0;
			float dist = 100000000.0f;

			for (unsigned int i = 0; i < images.size(); i++) {
				float d = size(images.at(i).pos - coord);
				if (d < dist) {
					s = i;
					dist = d;
				}
			}

			selected = s;
		}
	}



	void updateText() {

		Vector2f mousePos = target->mapPixelToCoords(sf::Mouse::getPosition(*target), mapView);

		std::stringstream ss;

		//ss.precision(0);

		if (state == 0) {
			ss << "radie: " << staticBoxes.at(selected).r;
		} else {
			ss << "image: " << images.at(selected).filename;
		}

		ss << "\n";

		ss << 
		"\nstate: " << state << "\nselected: " << selected << 
		"\nx: " << (int)mousePos.x << "\ny: " << (int)mousePos.y << 
		"\n\n";


        infoText.setString(ss.str());




	}



	int addImage(ImageM img) {
		for (unsigned int i = 0; i < images.size(); i++) {
			if (img.pos.y < images.at(i).pos.y) {
				images.insert(images.begin() + i, img);
				return i;
			}
		}

		images.push_back(img);
		return images.size() - 1;
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


				addImage(ImageM(Vector2f(x, y), str1));
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
				images.at(i).filename.c_str(), images.at(i).pos.x, images.at(i).pos.y) < 1) {

				std::cout << "Error writeing to file: " << path << std::endl;
				exit(-1);
			}
		}
		fclose(file);
	}
};

#endif
