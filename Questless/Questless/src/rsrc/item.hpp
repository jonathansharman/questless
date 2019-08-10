//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "utility.hpp"

#include <SFML/Graphics.hpp>

namespace ql::rsrc {
	//! Contains textures for item animations.
	struct item {
		sf::Texture error = load<sf::Texture>("resources/textures/items/error.png");

		sf::Texture arrow = load<sf::Texture>("resources/textures/items/arrow.png");
		sf::Texture bow = load<sf::Texture>("resources/textures/items/bow.png");
		sf::Texture blank_scroll = load<sf::Texture>("resources/textures/items/blank-scroll.png");
		sf::Texture charged_gatestone = load<sf::Texture>("resources/textures/items/charged-gatestone.png");
		sf::Texture quarterstaff = load<sf::Texture>("resources/textures/items/quarterstaff.png");
		sf::Texture quiver = load<sf::Texture>("resources/textures/items/quiver.png");
		sf::Texture uncharged_gatestone = load<sf::Texture>("resources/textures/items/uncharged-gatestone.png");
		sf::Texture written_scroll = load<sf::Texture>("resources/textures/items/written-scroll.png");
	};
}
