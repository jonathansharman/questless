//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "widget.hpp"

namespace ql {
	//! A UI element.
	struct inventory_widget : widget {
		inventory_widget(widget& parent) : widget{&parent} {}

		view::vector get_local_offset() const final;

		view::vector get_size() const final;

		void update(sec elapsed_time, std::vector<sf::Event>& events) final;

		void draw(sf::RenderTarget& target, sf::RenderStates states) const final;

	private:
		bool _inv_open = false;
		int _inv_page = 0; //! @todo Replace with filters and a scrollable view.
		sf::IntRect _inv_layout;
		int _inv_row_count;
		int _inv_column_count;
		std::vector<ent> _displayed_items;
	};
}
