//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "spell_fwd.hpp"
#include "utility.hpp"

#include <SFML/Graphics.hpp>
#include <gsl/pointers>

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

	using spell_ptr = gsl::not_null<spell const*>;
}
