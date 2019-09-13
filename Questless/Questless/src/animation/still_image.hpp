//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "animation.hpp"

#include <SFML/Graphics.hpp>

namespace ql {
	//! An animation composed of a single still image.
	struct still_image : animation {
		//! @param texture_handle A handle to the texture to use for this still image.
		still_image(sf::Texture const& texture);

		//! Sets the origin as a multiple of the texture size. E.g., (0, 1) is bottom-center.
		//! @param relative_origin The origin as a multiple of the texture size.
		//! @param round If true, the origin coordinates will be rounded to whole numbers.
		auto set_relative_origin(sf::Vector2f relative_origin, bool round = false) -> void;

		//! Sets the color of this still's sprite to @p color.
		auto set_color(sf::Color color) -> void;

	private:
		sf::Sprite _sprite;

		auto animation_subupdate(sec) -> void final {}

		auto animation_subdraw(sf::RenderTarget& target, sf::RenderStates states) const -> void final;
	};
}
