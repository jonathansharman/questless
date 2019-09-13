//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "animation.hpp"

#include "utility/reference.hpp"

namespace ql {
	//! An animation composed of a single still shape.
	struct still_shape : animation {
		uptr<sf::Shape> shape;

		still_shape(uptr<sf::Shape> shape) : shape{std::move(shape)} {}

	private:
		auto animation_subupdate(sec) -> void final {}

		auto animation_subdraw(sf::RenderTarget& target, sf::RenderStates states) const -> void final;
	};
}
