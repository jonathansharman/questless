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
		split_panel(widget& parent, ql::split_line split_line);

		view::vector get_size() const final;

		void update(sec elapsed_time, std::vector<sf::Event>& events) final;

		void draw(sf::RenderTarget& target, sf::RenderStates states) const final;

		//! Sets the first child widget to @p first_child.
		void set_first_child(widget* first_child);

		//! Sets the second child widget to @p second_child and .
		void set_second_child(widget* second_child);

	private:
		widget* _first;
		widget* _second;

		void arrange_first();
		void arrange_second();
	};
}
