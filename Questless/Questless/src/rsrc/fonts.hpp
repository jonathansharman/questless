//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "utility.hpp"

#include <SFML/Graphics.hpp>

namespace ql::rsrc {
	//! Contains all the fonts.
	struct fonts {
		sf::Font dumbledor1 = load<sf::Font>("resources/fonts/dumbledor1.ttf");
		sf::Font firamono = load<sf::Font>("resources/fonts/firamono.ttf");
	};
}
