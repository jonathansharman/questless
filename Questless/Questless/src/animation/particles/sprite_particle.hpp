//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "particle.hpp"

namespace ql {
	//! A particle drawn with a sprite.
	struct sprite_particle : particle {
		//! @param lifetime The amount of time before this particle expires.
		//! @param texture The texture to use for this particle's sprite.
		sprite_particle(sec lifetime, sf::Texture const& texture);

		virtual ~sprite_particle() = default;

	private:
		sf::Sprite _sprite;

		auto particle_subupdate(sec elapsed_time) -> void final;

		auto particle_subdraw(sf::RenderTarget& target, sf::RenderStates states) const -> void final;

		//! Sprite particle subtype-specific update.
		virtual auto sprite_particle_subupdate(sec elapsed_time) -> void = 0;
	};
}
