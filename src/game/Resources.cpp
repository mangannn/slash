
#include <iostream>
#include <map>
#include <SFML/Graphics.hpp>

#include "Resources.hpp"

static std::map<std::string, sf::Texture*> texture_list;

sf::Texture *Resources::getTexture(const std::string filename) {
	if (texture_list.find(filename) == texture_list.end()) {
		sf::Texture *texture = new sf::Texture();
		if (!texture->loadFromFile(filename)) {
			//std::cout << "Missing texture: " << filename << std::endl;
			// loadFromFile already prints if it dont find the texture
			texture->loadFromFile("media/images/no-texture.png");
		}
		texture_list[filename] = texture;
		return texture;
	} else {
		return texture_list[filename];
	}
}

static sf::Font _RESOURCES_mainFont;

void Resources::setMainFont(sf::String filename){
	_RESOURCES_mainFont.loadFromFile(filename);
}

sf::Font *Resources::getMainFont(){
	return &_RESOURCES_mainFont;
}
