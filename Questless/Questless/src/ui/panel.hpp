//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "widget.hpp"

namespace ql {
	//! Holds any number of elements, with arbitrary offsets.
	struct panel : widget {
		//! The widgets contained in this panel and their offsets.
		std::vector<std::tuple<widget*, view::vector>> children;

		panel();

		auto get_size() const -> view::vector final;

		auto update(sec elapsed_time) -> void final;

		auto set_position(view::point position) -> void final;

		auto get_position() const -> view::point final;

		auto on_parent_resize(view::vector parent_size) -> void final;

	private:
		view::point _position;
		view::vector _size;

		auto draw(sf::RenderTarget& target, sf::RenderStates states) const -> void final;
	};
}
