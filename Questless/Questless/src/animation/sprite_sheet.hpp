//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <SFML/Graphics.hpp>

namespace ql {
	//! A texture composed of a grid of animation cels.
	struct sprite_sheet {
		//! A reference to this sprite sheet's texture.
		sf::Texture const& texture;

		//! The dimensions of this sprite sheet's cel grid.
		sf::Vector2i cel_grid_dimensions;

		//! The dimensions of a single cel.
		auto cel_size() const {
			auto const size = texture.getSize();
			return sf::Vector2i{
				static_cast<int>(size.x) / cel_grid_dimensions.x, static_cast<int>(size.y) / cel_grid_dimensions.y};
		}

		//! The rectangle of the sprite cel at the given @p cel_coords.
		auto get_cel_rect(sf::Vector2i cel_coords) const {
			auto const size = cel_size();
			return sf::IntRect{size.x * cel_coords.x, size.y * cel_coords.y, size.x, size.y};
		}
	};
}
