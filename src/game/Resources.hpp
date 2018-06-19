#ifndef _Resources_
#define _Resources_

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

namespace Resources {

	sf::Texture *getTexture(const std::string filename);

	void setMainFont(sf::String);
	sf::Font *getMainFont();

}

#endif
