//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <array>
#include <optional>
#include <vector>

#include "items/item.hpp"
#include "sdl/input.hpp"
#include "sdl/font.hpp"
#include "sdl/texture.hpp"
#include "utility/id.hpp"

namespace ql
{
	class being;

	//! The head-up display controls and displays various elements, such as conditions, the hotbar, and the inventory.
	class hud
	{
	public:
		hud();

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
		// Constants

		static constexpr float _inv_width_pct = 0.7f;
		static constexpr float _inv_height_pct = 0.7f;

		static constexpr units::screen_space::vector _item_icon_size{55, 55};
		static constexpr int _item_count_font_size = 10;

		static constexpr size_t _hotbar_size = 10;
		static constexpr int _hotbar_interslot_gap = 2;
		static constexpr int _hotbar_bottom_gap = 2;
		static constexpr int _hotbar_slot_width = 65;
		static constexpr int _hotbar_slot_h_padding = 5;
		static constexpr int _hotbar_slot_v_padding = 5;

		static constexpr int _conditions_count = 2;
		static constexpr int _condition_bar_width = 10;
		static constexpr int _condition_bar_height = 100;

		// State Data

		std::optional<id<being>> _player_being_id = std::nullopt;
		std::array<std::optional<id<item>>, _hotbar_size> _hotbar;
		bool _inv_open = false;
		int _inv_page = 0; //! @todo Replace with filters.
		std::vector<cref<item>> _displayed_items;

		// View Data

		int _screen_bottom;

		int _hotbar_width;
		int _hotbar_x_start;

		units::screen_space::box _inv_layout;

		int _inv_row_count;
		int _inv_column_count;

		uptr<sdl::texture> _hotbar_slot_texture;

		uptr<sdl::font> _fnt_item_count;

		// Methods

		void load_textures();
		void load_layout();

		void update_displayed_items(being const& player_being);
	};
}
