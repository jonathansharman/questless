//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "utility.hpp"

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

namespace ql::rsrc {
	//! Contains resources for the world renderer.
	struct world_renderer {
		struct {
			//! @todo This is a placeholder. Add an arrow-hit sound.
			sf::SoundBuffer arrow_buffer = load<sf::SoundBuffer>("resources/sounds/spells/telescope.wav");
			sf::Sound arrow{arrow_buffer};

			sf::SoundBuffer hit_buffer = load<sf::SoundBuffer>("resources/sounds/weapons/hit.wav");
			sf::Sound hit{hit_buffer};

			sf::SoundBuffer pierce_buffer = load<sf::SoundBuffer>("resources/sounds/weapons/pierce.wav");
			sf::Sound pierce{pierce_buffer};

			sf::SoundBuffer shock_buffer = load<sf::SoundBuffer>("resources/sounds/spells/lightning-bolt.wav");
			sf::Sound shock{shock_buffer};

			sf::SoundBuffer telescope_buffer = load<sf::SoundBuffer>("resources/sounds/spells/telescope.wav");
			sf::Sound telescope{telescope_buffer};
		} sfx;
	};
}
