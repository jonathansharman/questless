//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "still_image.hpp"

#include <SFML/Graphics/Texture.hpp>

namespace ql {
	still_image::still_image(sf::Texture const& texture) : _sprite{texture} {}

	auto still_image::set_relative_origin(sf::Vector2f relative_origin, bool truncate) -> void {
		auto const size = _sprite.getTexture()->getSize();
		if (truncate) {
			setOrigin(roundf(size.x * relative_origin.x), roundf(size.y * relative_origin.y));
		} else {
			setOrigin(size.x * relative_origin.x, size.y * relative_origin.y);
		}
	}

	auto still_image::animation_subdraw(sf::RenderTarget& target, sf::RenderStates states) const -> void {
		target.draw(_sprite, states);
	}

	auto still_image::set_color(sf::Color color) -> void {
		_sprite.setColor(color);
	}
}
