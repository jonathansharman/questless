//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "particle_fwd.hpp"
#include "utility.hpp"

#include <SFML/Graphics.hpp>

namespace ql::rsrc {
	//! Contains the textures used for particle animations.
	struct particle {
		sf::Texture white_magic = load<sf::Texture>("resources/textures/particles/magic/white.png");
		sf::Texture black_magic = load<sf::Texture>("resources/textures/particles/magic/black.png");
		sf::Texture green_magic = load<sf::Texture>("resources/textures/particles/magic/green.png");
		sf::Texture red_magic = load<sf::Texture>("resources/textures/particles/magic/red.png");
		sf::Texture blue_magic = load<sf::Texture>("resources/textures/particles/magic/blue.png");
		sf::Texture yellow_magic = load<sf::Texture>("resources/textures/particles/magic/yellow.png");

		sf::Texture arrow = load<sf::Texture>("resources/textures/particles/arrow.png");
		sf::Texture blood = load<sf::Texture>("resources/textures/particles/blood.png");
		sf::Texture glow_small = load<sf::Texture>("resources/textures/particles/glow-small.png");
	};
}
