//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "utility.hpp"

#include <SFML/Graphics.hpp>

namespace ql::rsrc {
	//! Contains textures for spell animations.
	struct spell {
		struct {
			sf::Texture heal = load<sf::Texture>("resources/textures/textures/spells/heal.png");
			sf::Texture shock = load<sf::Texture>("resources/textures/textures/spells/shock.png");
			sf::Texture telescope = load<sf::Texture>("resources/textures/textures/spells/telescope.png");
			sf::Texture teleport = load<sf::Texture>("resources/textures/textures/spells/teleport.png");
		} txtr;
	};
}
