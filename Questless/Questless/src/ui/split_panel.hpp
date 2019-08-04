//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "widget.hpp"

#include <variant>

namespace ql {
	enum class split_line { horizontal, vertical };

	//! Holds two elements that share.
	struct split_panel : widget {
		//! The orientation of the split line.
		split_line split_line;

		//! Where the first child ends and the second begins, either as a fixed offset or as a factor of the total length.
		std::variant<view::px, float> split_location = 0.5f;

		//! @param split_line The proportion of the panel's area that should be held by the first child.
		split_panel(ql::split_line split_line);

		auto get_size() const -> view::vector final;

		auto update(sec elapsed_time) -> void final;

		auto set_position(view::point position) -> void final;

		auto get_position() const -> view::point final;

		auto on_parent_resize(view::vector parent_size) -> void final;

		//! Sets the first child widget to @p first_child and arranges it.
		auto set_first_child(widget* first_child) -> void;

		//! Sets the second child widget to @p second_child and arranges it.
		auto set_second_child(widget* second_child) -> void;

	private:
		view::point _position;
		view::vector _size;

		widget* _first;
		widget* _second;

		auto draw(sf::RenderTarget& target, sf::RenderStates states) const -> void final;

		//! Calls the first child's @p on_parent_resize and sets its position.
		void arrange_first();

		//! Calls the second child's @p on_parent_resize and sets its position.
		void arrange_second();
	};
}
