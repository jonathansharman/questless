/**
* @file    HUDView.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The HUDView class, which is responsible for drawing the HUD.
*/

#pragma once

#include "sdl/Renderable.h"
#include "sdl/Font.h"
#include "sdl/Window.h"
#include "ui/HUDModel.h"

namespace questless
{
	class HUDView : public sdl::Renderable
	{
	public:
		static constexpr double inv_width_percent = 0.7;
		static constexpr double inv_height_percent = 0.7;

		static constexpr int item_icon_width = 55;
		static constexpr int item_icon_height = 55;

		HUDView();

		/// @return The inventory's width.
		int inv_width() const { return _inv_width; }

		/// @return The inventory's height.
		int inv_height() const { return _inv_height; }

		/// @return The x-coordinate of the inventory's left edge.
		int inv_left() const { return _inv_left; }

		/// @return The y-coordinate of the inventory's top edge.
		int inv_top() const { return _inv_top; }

		/// @return The number of item columns in the inventory.
		int inv_column_count() const { return _inv_column_count; }

		/// Draws the given HUD.
		/// @param hud The HUD to be drawn.
		void draw(HUDModel const& hud);
	private:
		static const int _item_count_font_size = 10;
		
		static constexpr int _hotbar_interslot_gap = 2;
		static constexpr int _hotbar_bottom_gap = 2;
		static constexpr int _hotbar_slot_width = 65;
		static constexpr int _hotbar_slot_h_padding = 5;
		static constexpr int _hotbar_slot_v_padding = 5;

		static constexpr int _conditions_count = 2;
		static constexpr int _condition_bar_width = 10;
		static constexpr int _condition_bar_height = 100;

		int _screen_bottom;

		int _hotbar_width;
		int _hotbar_x_start;

		int _inv_left;
		int _inv_top;

		int _inv_width;
		int _inv_height;

		int _inv_column_count;

		sdl::Texture::uptr _hotbar_slot_texture;

		sdl::Font::uptr _fnt_item_count;

		void refresh() override { load_textures_and_layout(); }

		void load_textures_and_layout();
	};
}
