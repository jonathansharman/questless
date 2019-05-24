//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "widget.hpp"

#include "ui/view_space.hpp"
#include "utility/reference.hpp"

#include <array>
#include <optional>
#include <vector>

namespace ql {
	//! The head-up display controls and displays various elements, such as conditions, the hotbar, and the inventory.
	struct hud : widget {
		//! @param player_being_id The ID of the player-controlled being.
		hud(ent player_being_id) : _player_being_id{player_being_id} {}

		void update(sec elapsed_time, input_manager& im) final;

		void draw(sf::RenderTarget& target, sf::RenderStates states) const final;

		//! A constant reference to the hotbar.
		auto const& hotbar() {
			return _hotbar;
		}

		//! Whether the inventory view is open.
		bool inventory_open() const {
			return _inv_open;
		}

	private:
		static constexpr size_t _hotbar_size = 10;

		ent _player_being_id;
		std::array<std::optional<ent>, _hotbar_size> _hotbar;
		bool _inv_open = false;
		int _inv_page = 0; //! @todo Replace with filters and a scrollable view.
		sf::IntRect _inv_layout;
		int _inv_row_count;
		int _inv_column_count;
		std::vector<ent> _displayed_items;

		void update_displayed_items(being const& player_being);
	};
}
