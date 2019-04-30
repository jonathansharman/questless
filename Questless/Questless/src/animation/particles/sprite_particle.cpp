//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "sprite_particle.hpp"

using namespace media;

namespace ql {
	sprite_particle::sprite_particle(sec lifetime, sf::Texture const& texture) : particle{lifetime}, _sprite{texture} {
		// Set origin to the center of the sprite's texture by default.
		setOrigin(sf::Vector2f{texture.getSize()} / 2.0f);
	}

	void sprite_particle::particle_subupdate(sec elapsed_time) {
		_sprite.setColor(color_factor);
		sprite_particle_subupdate(elapsed_time);
	}

	void sprite_particle::particle_subdraw(sf::RenderTarget& target, sf::RenderStates states) const {
		target.draw(_sprite, states);
	}
}
