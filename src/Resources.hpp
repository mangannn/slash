#ifndef _RESOURCES_H_
#define _RESOURCES_H_

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

namespace Resources {

	sf::Texture *getTexture(const std::string filename);

	void setMainFont(sf::String);
	sf::Font *getMainFont();

}

#endif
