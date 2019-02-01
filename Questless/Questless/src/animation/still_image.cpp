//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "animation/still_image.hpp"

#include <SFML/Graphics/Texture.hpp>

namespace ql {
	still_image::still_image(sf::Texture const& texture) : _sprite{texture} {}

	void still_image::set_relative_origin(sf::Vector2f relative_origin) {
		auto const size = _sprite.getTexture()->getSize();
		setOrigin(size.x * relative_origin.x, size.y * relative_origin.y);
	}

	void still_image::animation_subdraw(sf::RenderTarget& target, sf::RenderStates states) const {
		target.draw(_sprite, states);
	}

	void still_image::set_color(sf::Color color) {
		_sprite.setColor(color);
	}
}
