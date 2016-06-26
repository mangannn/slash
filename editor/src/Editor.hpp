#ifndef _Editor_H_
#define _Editor_H_

#include <string.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include <SFML/Graphics.hpp>

#include "Collision.hpp"


#include "Resources.hpp"


#define SELECTION_SHIFT (3)


class Background {

public:

	Vector2f pos;

	sf::Sprite sprite;

	std::string filename;

	Background(Vector2f position, sf::String filenameParam):
		pos(position),
		filename(filenameParam)
	{

		sf::Texture *texture = Resources::getTexture(filename);
		sprite.setTexture(*texture);
		sprite.setOrigin(sf::Vector2f((float)texture->getSize().x, (float)texture->getSize().y) / 2.0f);
	}

	virtual void draw(RenderTarget *target) {
		sprite.setPosition(pos);
		target->draw(sprite);
	}
};

class ImageM {

public:

	Vector2f pos;

	sf::Sprite sprite;

	std::string filename;

	ImageM(Vector2f position, sf::String filenameParam):
		pos(position),
		filename(filenameParam)
	{

		sf::Texture *texture = Resources::getTexture(filename);
		sprite.setTexture(*texture);
		sprite.setOrigin(sf::Vector2f((float)texture->getSize().x / 2.0f, (float)texture->getSize().y));
		//pos.y += (float)texture->getSize().y / 2.0f;
	}

	virtual void draw(RenderTarget *target) {
		sprite.setPosition(pos);
		target->draw(sprite);
	}
};

class Spawn {

public:

	Vector2f pos;


	std::string type;

	float r;

	Spawn() {}
	Spawn(Vector2f position, std::string typeParam, float radius):
		pos(position),
		type(typeParam),
		r(radius)
	{
	}

	void draw(RenderTarget *target) {

		// for debug
		sf::CircleShape shape(r);
		shape.setFillColor(sf::Color(0, 0, 0, 0));
		shape.setOutlineThickness(1);
		shape.setOutlineColor(sf::Color(0, 250, 50));
		shape.setPosition(pos - Vector2f(r, r));
		target->draw(shape);
	}
};


class Editor {

public:

	std::string mapFilename;

	sf::View mapView, guiView;

	sf::Texture mapTex;
	sf::Sprite mapSprite;

	std::vector<Background> bgs;
	std::vector<ImageM> images;
	std::vector<CollisionBox> staticBoxes;
	std::vector<Spawn> spawns;


	Vector2f *origo = new Vector2f(0,0);


	bool mouseMove = false;
	Vector2f mouseMoveVec;

	int selected = 0;
	sf::RectangleShape selectedRect;

	const int numStates = 4;
	int state = 0;


	sf::Text infoText, helpText;

	bool typing = false;
	sf::Text typeText;



	sf::RenderWindow *target;

	Editor(sf::RenderWindow *targetParam): 
		target(targetParam)
	{


		std::ifstream file ("media/editor-settings.txt");
		if (file.is_open()) {
			if (!getline(file, mapFilename)) {
				std::cout << "Unable to read settings file!\n";
				mapFilename = "media/maps/map.txt";
			}
			file.close();
		} else {
			std::cout << "Unable to read settings file!\n";
			mapFilename = "media/maps/map.txt";
		}

		std::cout << "Loading map: " << mapFilename << "\n";
		loadMap(mapFilename.c_str());

		selectedRect.setSize(sf::Vector2f(30, 30));
		selectedRect.setOrigin(sf::Vector2f((float)selectedRect.getSize().x / 2.0f, (float)selectedRect.getSize().y / 2.0f));
		selectedRect.setFillColor(sf::Color(0, 0, 0, 0));
		selectedRect.setOutlineThickness(5);
		selectedRect.setOutlineColor(sf::Color(255, 0, 0));


		infoText.setFont(*Resources::getMainFont());
		infoText.setCharacterSize(40);
		infoText.setPosition(20, 0);

		helpText.setFont(*Resources::getMainFont());
		helpText.setCharacterSize(40);
		helpText.setPosition(300, 0);

		helpText.setString(sf::String(
			"h:\tthis help text\ns:\tsave\n") + sf::String(
			"1:\tstate 0 (collision)\n2:\tstate 1 (image)\n3:\tstate 2 (background)\n4:\tstate 3 (spawn)\n") + sf::String(
			"z:\tdelete selected object\n") + sf::String(
			"i:\tinvert draw order\nreturn: change filename (images)\narrows: shift all object of current state\n\n") + sf::String(
			"Mouse:\n\n") + sf::String(
			"right:     move screen\nleft:      select object\nleft+ctrl:  move selected object\n") + sf::String(
			"left+shift: copy selected object\nwheel:     zoom\nwheel+alt:   change size (collision)"
			));
		
		typeText.setFont(*Resources::getMainFont());
		typeText.setCharacterSize(40);
		typeText.setPosition(20, 300);


		mapView.setCenter(0,0);
	}

	~Editor() {
		std::string filename = "media/maps/autosave.txt";
		saveMap(filename.c_str());
		std::cout << "Autosaved file: " << filename << "\n";
	}


	void eventHandle(sf::Event event) {

		if (!typing) {

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
								} else if (state == 1) {
									images.at(selected).pos = coord;

									// put it in the right order here
									ImageM img = images.at(selected);
									images.erase(images.begin() + selected);
									selected = addImage(img);
								} else if (state == 2) {
									bgs.at(selected).pos = coord;
								} else {
									spawns.at(selected).pos = coord;
								}
							} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
								if (state == 0) {
									staticBoxes.push_back(CollisionBox(origo, coord, staticBoxes.at(selected).r));
									selected = staticBoxes.size() - 1;
								} else if (state == 1) {
									selected = addImage(ImageM(coord, images.at(selected).filename));
								} else if (state == 2) {
									bgs.push_back(Background(coord, bgs.at(selected).filename));
									selected = bgs.size() - 1;
								} else {
									spawns.push_back(Spawn(coord, spawns.at(selected).type, spawns.at(selected).r));
									selected = spawns.size() - 1;
								}
							} else {
								selected = closest(coord);
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
						} else if (state == 3) {
							spawns.at(selected).r *= 1.f + (0.1f * event.mouseWheel.delta);
						}
					} else {
						
						sf::Vector2f mousePosBefore = target->mapPixelToCoords(sf::Mouse::getPosition(*target), mapView);
						mapView.zoom(1.f + (0.1f * event.mouseWheel.delta));
						mapView.move(mousePosBefore - target->mapPixelToCoords(sf::Mouse::getPosition(*target), mapView));
						
					}
				} break;			
				case sf::Event::KeyPressed: {

					switch (event.key.code) {
						case sf::Keyboard::Return: {
							if (state == 1) {
								typing = true;
								typeText.setString(images.at(selected).filename);
							} else if (state == 2) {
								typing = true;
								typeText.setString(bgs.at(selected).filename);
							} else if (state == 3) {
								typing = true;
								typeText.setString(spawns.at(selected).type);
							}
						} break;
						case sf::Keyboard::Z: {
							if (state == 0) {
								if (staticBoxes.size() > 1) {
									staticBoxes.erase(staticBoxes.begin() + selected);
								}
							} else if (state == 1) {
								if (images.size() > 1) {
									images.erase(images.begin() + selected);
								}
							} else if (state == 2) {
								if (bgs.size() > 1) {
									bgs.erase(bgs.begin() + selected);
								}
							} else {
								if (spawns.size() > 1) {
									spawns.erase(spawns.begin() + selected);
								}
							}
							selected = closest(target->mapPixelToCoords(sf::Mouse::getPosition(*target), mapView));
						} break;
						case sf::Keyboard::S: {
							saveMap(mapFilename.c_str());
							std::cout << "Saved file: " << mapFilename << "\n";
						} break;
						case sf::Keyboard::Tab: {

							state += 1;
							if (state >= numStates) {
								state -= numStates;
							}
							selected = closest(target->mapPixelToCoords(sf::Mouse::getPosition(*target), mapView));
						} break;
						case sf::Keyboard::Num1: {
							state = 0;
							selected = closest(target->mapPixelToCoords(sf::Mouse::getPosition(*target), mapView));
						} break;
						case sf::Keyboard::Num2: {
							state = 1;
							selected = closest(target->mapPixelToCoords(sf::Mouse::getPosition(*target), mapView));
						} break;
						case sf::Keyboard::Num3: {
							state = 2;
							selected = closest(target->mapPixelToCoords(sf::Mouse::getPosition(*target), mapView));
						} break;
						case sf::Keyboard::Num4: {
							state = 3;
							selected = closest(target->mapPixelToCoords(sf::Mouse::getPosition(*target), mapView));
						} break;

						case sf::Keyboard::Left: {
							shiftState(Vector2f(-SELECTION_SHIFT, 0));
						} break;
						case sf::Keyboard::Right: {
							shiftState(Vector2f(SELECTION_SHIFT, 0));
						} break;
						case sf::Keyboard::Up: {
							shiftState(Vector2f(0, -SELECTION_SHIFT));
						} break;
						case sf::Keyboard::Down: {
							shiftState(Vector2f(0, SELECTION_SHIFT));
						} break;
						default: break;
					}
				} break;
				default: break;
			}
		} else {

			switch (event.type) {
				case sf::Event::KeyPressed: {
					switch (event.key.code) {
						case sf::Keyboard::Return: {
							typing = false;
							if (state == 1) {
								images.at(selected) = ImageM(images.at(selected).pos, typeText.getString());
							} else if (state == 2) {
								bgs.at(selected) = Background(bgs.at(selected).pos, typeText.getString());
							} else if (state == 3) {
								spawns.at(selected) = Spawn(spawns.at(selected).pos, typeText.getString(), spawns.at(selected).r);
							}
						} break;
						case sf::Keyboard::BackSpace: {
							sf::String s = typeText.getString();
							if (s.getSize() > 0) {
								s.erase(s.getSize() - 1);
								typeText.setString(s);
							}
						} break;
						default: break;
					}
				} break;
				case sf::Event::TextEntered: {
					if (event.text.unicode >= 32 && (event.text.unicode <= 126 || event.text.unicode >= 161)) {
						sf::String s = typeText.getString();
						s.insert(s.getSize(), event.text.unicode);
						typeText.setString(s);
					}
				} break;
				default: break;
			}
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
		

		for (unsigned int i = 0; i < bgs.size(); i++) {
			bgs.at(i).draw(target);
		}



		if (sf::Keyboard::isKeyPressed(sf::Keyboard::I)) {
			for (unsigned int i = images.size(); i > 0; i--) {
				images.at(i - 1).draw(target);
			}
		} else {
			for (unsigned int i = 0; i < images.size(); i++) {
				images.at(i).draw(target);
			}
		}


		for (unsigned int i = 0; i < staticBoxes.size(); i++) {
			staticBoxes.at(i).draw(target);
		}

		for (unsigned int i = 0; i < spawns.size(); i++) {
			spawns.at(i).draw(target);
		}



		target->setView(guiView);



		Vector2f coord;
		if (state == 0) {
			coord = staticBoxes.at(selected).pos;
		} else if (state == 1) {
			coord = images.at(selected).pos;
		} else if (state == 2) {
			coord = bgs.at(selected).pos;
		} else {
			coord = spawns.at(selected).pos;
		}

		selectedRect.setPosition(target->mapPixelToCoords(target->mapCoordsToPixel(coord, mapView), guiView));

		target->draw(selectedRect);

		updateText();
		target->draw(infoText);


		if (sf::Keyboard::isKeyPressed(sf::Keyboard::H)) {
			target->draw(helpText);
		}

		if (typing) {
			target->draw(typeText);
		}

	}






	int closest(Vector2f coord) {

		int s = 0;
		float dist = 100000000.0f;

		if (state == 0) {

			for (unsigned int i = 0; i < staticBoxes.size(); i++) {
				float d = size(staticBoxes.at(i).pos - coord);
				if (d < dist) {
					s = i;
					dist = d;
				}
			}

		} else if (state == 1) {

			for (unsigned int i = 0; i < images.size(); i++) {
				float d = size(images.at(i).pos - coord);
				if (d < dist) {
					s = i;
					dist = d;
				}
			}
		} else if (state == 2) {

			for (unsigned int i = 0; i < bgs.size(); i++) {
				float d = size(bgs.at(i).pos - coord);
				if (d < dist) {
					s = i;
					dist = d;
				}
			}
		} else {

			for (unsigned int i = 0; i < spawns.size(); i++) {
				float d = size(spawns.at(i).pos - coord);
				if (d < dist) {
					s = i;
					dist = d;
				}
			}
		}

		return s;
	}



	void updateText() {

		Vector2f mousePos = target->mapPixelToCoords(sf::Mouse::getPosition(*target), mapView);

		std::stringstream ss;

		//ss.precision(0);

		if (state == 0) {
			ss << "radie: " << staticBoxes.at(selected).r;
		} else if (state == 1) {
			ss << "image: " << images.at(selected).filename;
		} else if (state == 2) {
			ss << "background: " << bgs.at(selected).filename;
		} else {
			ss << "spawn type: " << spawns.at(selected).type;
		}

		ss << "\n";

		ss << 
		"\nfile: " << mapFilename <<
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



	void shiftState(Vector2f v) {

		if (state == 0) {
			for (unsigned int i = 0; i < staticBoxes.size(); i++) {
				staticBoxes.at(i).pos += v;
			}
		} else if (state == 1) {
			for (unsigned int i = 0; i < images.size(); i++) {
				images.at(i).pos += v;
			}
		} else if (state == 2) {
			for (unsigned int i = 0; i < bgs.size(); i++) {
				bgs.at(i).pos += v;
			}
		} else {
			for (unsigned int i = 0; i < spawns.size(); i++) {
				spawns.at(i).pos += v;
			}
		}
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
			} else if (strcmp(str1, "bg") == 0) {

				num_back = sscanf(str2, 
					"%[^,\n],%f,%f", 
					str1, &x, &y);

				if (num_back != 3) {
					continue;
				}


				bgs.push_back(Background(Vector2f(x, y), str1));
			} else if (strcmp(str1, "spawn") == 0) {

				num_back = sscanf(str2, 
					"%[^,\n],%f,%f,%f", 
					str1, &x, &y, &r);

				if (num_back != 4) {
					continue;
				}


				spawns.push_back(Spawn(Vector2f(x, y), str1, r));
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


		for (unsigned int i = 0; i < bgs.size(); i++) {

			if (fprintf(file, 
				"bg{%s, %f, %f}\n", 
				bgs.at(i).filename.c_str(), bgs.at(i).pos.x, bgs.at(i).pos.y) < 1) {

				std::cout << "Error writeing to file: " << path << std::endl;
				exit(-1);
			}
		}
		for (unsigned int i = 0; i < spawns.size(); i++) {

			if (fprintf(file, 
				"spawn{%s, %f, %f, %f}\n", 
				spawns.at(i).type.c_str(), spawns.at(i).pos.x, spawns.at(i).pos.y, spawns.at(i).r) < 1) {

				std::cout << "Error writeing to file: " << path << std::endl;
				exit(-1);
			}
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
