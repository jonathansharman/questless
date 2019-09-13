//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "entity.hpp"
#include "fonts.hpp"
#include "particle.hpp"
#include "tile.hpp"

#include <SFML/Audio.hpp>

namespace ql::rsrc {
	//! Contains resources for the world renderer.
	struct world_widget {
		entity const& entity;
		fonts const& fonts;
		particle const& particle;
		tile const& tile;

		struct {
			//! @todo This is a placeholder. Add an arrow-hit sound.
			sf::SoundBuffer arrow = load<sf::SoundBuffer>("resources/sounds/weapons/arrow.wav");
			sf::SoundBuffer hit = load<sf::SoundBuffer>("resources/sounds/weapons/hit.wav");
			sf::SoundBuffer pierce = load<sf::SoundBuffer>("resources/sounds/weapons/pierce.wav");
			sf::SoundBuffer shock = load<sf::SoundBuffer>("resources/sounds/spells/lightning-bolt.wav");
			sf::SoundBuffer telescope = load<sf::SoundBuffer>("resources/sounds/spells/telescope.wav");
		} sfx;
	};
}
