//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "utility/id.hpp"
#include "utility/reference.hpp"

#include "media/spaces/window.hpp"

#include <array>
#include <optional>
#include <vector>

namespace ql {
	struct being;
	struct item;

	//! The head-up display controls and displays various elements, such as conditions, the hotbar, and the inventory.
	struct hud {
		//! @param player_being_id The ID of the player-controlled being.
		hud(id<being> player_being_id) : _player_being_id{player_being_id} {}

		//! Updates the HUD's state and animations. Should be called once per frame.
		void update();

		//! Draws the HUD.
		void draw(sf::RenderTarget& target, sf::RenderStates states);

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

		id<being> _player_being_id;
		std::array<std::optional<id<item>>, _hotbar_size> _hotbar;
		bool _inv_open = false;
		int _inv_page = 0; //! @todo Replace with filters and a scrollable view.
		sf::IntRect _inv_layout;
		int _inv_row_count;
		int _inv_column_count;
		std::vector<cref<item>> _displayed_items;

		void update_displayed_items(being const& player_being);
	};
}
