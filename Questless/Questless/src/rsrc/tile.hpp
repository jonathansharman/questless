//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "utility.hpp"

#include <SFML/Graphics.hpp>

namespace ql::rsrc {
	//! Contains textures for tile animations.
	struct tile {
		struct {
			sf::Texture selector = load<sf::Texture>("resources/textures/terrain/selector.png");
		} ss;

		struct {
			sf::Texture blank = load<sf::Texture>("resources/textures/terrain/blank.png");
			sf::Texture dirt = load<sf::Texture>("resources/textures/terrain/dirt.png");
			sf::Texture grass = load<sf::Texture>("resources/textures/terrain/grass.png");
			sf::Texture sand = load<sf::Texture>("resources/textures/terrain/sand.png");
			sf::Texture snow = load<sf::Texture>("resources/textures/terrain/snow.png");
			sf::Texture stone = load<sf::Texture>("resources/textures/terrain/stone.png");
			sf::Texture water = load<sf::Texture>("resources/textures/terrain/water.png");
		} txtr;
	};
}
