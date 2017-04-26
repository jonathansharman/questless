//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <array>
#include <optional>
#include <vector>

#include "items/Item.h"
#include "sdl/Input.h"
#include "sdl/Renderable.h"
#include "sdl/Texture.h"
#include "utility/Id.h"

namespace questless
{
	class Being;

	//! The head-up display controls and displays various elements, such as conditions, the hotbar, and the inventory.
	class HUD : public sdl::Renderable
	{
	public:
		HUD();

		//! Updates the HUD's state and animations. Should be called once per frame.
		void update();

		//! Sets the HUD's associated player being ID to the given ID.
		//! @param player_being_id The ID of the player-controlled being.
		void set_player_being_id(Id<Being> player_being_id);

		//! Draws the HUD.
		void draw();

		//! A constant reference to the hotbar.
		auto const& hotbar() { return _hotbar; }

		//! Whether the inventory view is open.
		bool inventory_open() const { return _inv_open; }
	private:
		// Constants

		static constexpr double _inv_width_percent = 0.7;
		static constexpr double _inv_height_percent = 0.7;

		static constexpr int _item_icon_width = 55;
		static constexpr int _item_icon_height = 55;
		static const int _item_count_font_size = 10;

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

		std::optional<Id<Being>> _player_being_id = std::nullopt;
		std::array<std::optional<Id<Item>>, _hotbar_size> _hotbar;
		bool _inv_open = false;
		int _inv_page = 0; //! @todo Replace with filters.
		std::vector<cref<Item>> _displayed_items;

		// View Data

		int _screen_bottom;

		int _hotbar_width;
		int _hotbar_x_start;

		int _inv_left;
		int _inv_top;

		int _inv_width;
		int _inv_height;
		int _inv_row_count;
		int _inv_column_count;

		uptr<sdl::Texture> _hotbar_slot_texture;

		uptr<sdl::Font> _fnt_item_count;

		// Methods

		void refresh() override { load_textures_and_layout(); }

		void load_textures_and_layout();

		void update_displayed_items(Being& player_being);
	};
}
