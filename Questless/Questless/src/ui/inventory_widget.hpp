//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "widget.hpp"

#include "items/inventory.hpp"

namespace ql {
	//! A UI element.
	struct inventory_widget : widget {
		//! @param inventory The inventory this widget will be used to interact with.
		inventory_widget(widget& parent, inventory& inventory);

		view::vector get_size() const final;

		void update(sec elapsed_time, std::vector<sf::Event>& events) final;

		void draw(sf::RenderTarget& target, sf::RenderStates states) const final;

	private:
		inventory& _inv;
		int _inv_page = 0; //! @todo Replace with filters and a scrollable view.
		int _inv_row_count;
		int _inv_column_count;
		std::vector<ent> _displayed_items;
	};
}
