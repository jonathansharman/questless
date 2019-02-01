//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <SFML/Graphics.hpp>

namespace ql {
	//! A texture composed of a grid of animation cels.
	class sprite_sheet {
	public:
		//! A reference to this sprite sheet's texture.
		sf::Texture const& texture;

		//! The dimensions of this sprite sheet's cel grid.
		sf::Vector2i cel_grid_dimensions;

		//! The dimensions of a single cel.
		sf::Vector2i cel_size() const {
			return {
				static_cast<int>(texture.getSize().x) / cel_grid_dimensions.x,
				static_cast<int>(texture.getSize().y) / cel_grid_dimensions.y
			};
		}

		//! The rectangle of the sprite cel at the given @p cel_coords.
		sf::IntRect get_cel_rect(sf::Vector2i cel_coords) const {
			auto const size = cel_size();
			return sf::IntRect{size.x * cel_coords.x, size.y * cel_coords.y, size.x, size.y};
		}
	};
}
