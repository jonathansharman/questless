//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "particle.hpp"

namespace ql {
	//! A particle drawn with a sprite.
	class sprite_particle : public particle {
	public:
		//! @param displacement The initial displacement of this particle.
		//! @param lifetime The amount of time before this particle expires.
		//! @param texture The texture to use for this particle's sprite.
		sprite_particle(world::vector displacement, sec lifetime, sf::Texture const& texture);

		virtual ~sprite_particle() = default;

	private:
		sf::Sprite _sprite;

		void particle_subupdate(sec elapsed_time) final;

		void particle_subdraw(sf::RenderTarget& target, sf::RenderStates states) const final;

		//! Sprite particle subtype-specific update.
		virtual void sprite_particle_subupdate(sec elapsed_time) = 0;
	};
}
