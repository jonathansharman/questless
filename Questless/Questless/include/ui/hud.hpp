//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <array>
#include <optional>
#include <vector>

#include "units/window_space.hpp"
#include "utility/id.hpp"
#include "utility/reference.hpp"

namespace ql
{
	class being;
	class item;

	//! The head-up display controls and displays various elements, such as conditions, the hotbar, and the inventory.
	class hud
	{
	public:
		//! Updates the HUD's state and animations. Should be called once per frame.
		void update();

		//! Sets the HUD's associated player being ID to the given ID.
		//! @param player_being_id The ID of the player-controlled being.
		void set_player_being_id(id<being> player_being_id);

		//! Draws the HUD.
		void draw();

		//! A constant reference to the hotbar.
		auto const& hotbar() { return _hotbar; }

		//! Whether the inventory view is open.
		bool inventory_open() const { return _inv_open; }
	private:
		static constexpr size_t _hotbar_size = 10;

		std::optional<id<being>> _player_being_id = std::nullopt;
		std::array<std::optional<id<item>>, _hotbar_size> _hotbar;
		bool _inv_open = false;
		int _inv_page = 0; //! @todo Replace with filters and a scrollable view.
		units::window_space::box _inv_layout;
		int _inv_row_count;
		int _inv_column_count;
		std::vector<cref<item>> _displayed_items;

		void update_displayed_items(being const& player_being);
	};
}
