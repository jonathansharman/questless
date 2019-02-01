//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "animation.hpp"

namespace sf {
	class Texture;
}

namespace ql {
	//! An animation composed of a single still image.
	class still_image : public animation {
	public:
		//! @param texture_handle A handle to the texture to use for this still image.
		still_image(sf::Texture const& texture);

		//! Sets the origin as a multiple of the texture size. E.g., (0, 1) is bottom-center.
		//! @param relative_origin The origin as a multiple of the texture size.
		void set_relative_origin(sf::Vector2f relative_origin);

		//! Sets the color of this still's sprite to @p color.
		void set_color(sf::Color color);

	private:
		sf::Sprite _sprite;

		void animation_subupdate(sec) final {}

		void animation_subdraw(sf::RenderTarget& target, sf::RenderStates states) const final;
	};
}
