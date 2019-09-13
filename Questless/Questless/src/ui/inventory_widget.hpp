//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "widget.hpp"

#include "items/inventory.hpp"

namespace ql {
	struct hotbar;

	//! Interfaces with an inventory.
	struct inventory_widget : widget {
		inventory_widget(inventory& inventory, hotbar& hotbar);

		auto get_size() const -> view::vector final;

		auto update(sec elapsed_time) -> void final;

		auto set_position(view::point position) -> void final;

		auto get_position() const -> view::point final;

		auto on_parent_resize(view::vector parent_size) -> void final;

		auto on_key_press(sf::Event::KeyEvent const&) -> event_handled final;

		auto on_mouse_move(view::point mouse_position) -> void final;

	private:
		inventory& _inv;
		hotbar& _hotbar;
		view::point _position;
		view::vector _size;
		int _inv_page = 0; //! @todo Replace with filters and a scrollable view.
		int _row_count;
		int _col_count;
		std::vector<id> _displayed_items;
		view::point _mouse_position;

		auto draw(sf::RenderTarget& target, sf::RenderStates states) const -> void final;

		auto assign_idx(size_t hotbar_idx) -> void;
	};
}
