
#include <map>
#include <SFML/Graphics.hpp>

static std::map<std::string, sf::Texture*> texture_list;

sf::Texture *getTexture(const std::string filename) {
	if (texture_list.find(filename) == texture_list.end()) {
		sf::Texture *texture = new sf::Texture();
		texture->loadFromFile(filename);
		texture_list[filename] = texture;
		return texture;
	} else {
		return texture_list[filename];
	}
}

sf::Font _RESOURCES_mainFont;

void setMainFont(sf::String filename){
    _RESOURCES_mainFont.loadFromFile(filename);
}

sf::Font *getMainFont(){
    return &_RESOURCES_mainFont;
}
