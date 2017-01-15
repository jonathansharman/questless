/**
* @file    HUDView.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the HUDView class.
*/

#include "ui/HUDView.h"
#include "Game.h"
#include "entities/beings/Being.h"
#include "animation/ItemTexturer.h"
#include "animation/BodyTexturer.h"

using namespace sdl;
using namespace units;

namespace questless
{
	HUDView::HUDView(Game& game, const Window& window) : _game{game}, _window{window}
	{
		_fnt_item_count = std::make_unique<Font>("resources/fonts/dumbledor1.ttf", _item_count_font_size, SDL_BLENDMODE_BLEND);

		load_textures_and_layout();
	}

	void HUDView::draw(const HUDModel& hud)
	{
		if (!hud.player_id) {
			return;
		}
		if (Being* player_being = _game.being(*hud.player_id)) {
			// Draw the condition bars.
			{
				int left = 0;
				renderer().draw_rect(ScreenRect{left, _screen_bottom, _condition_bar_width * _conditions_count, _condition_bar_height, ScreenRect::Origin::lower_left}, Color::black(), true);
				// Health
				int health_bar_height = static_cast<int>(_condition_bar_height * player_being->health() / player_being->vitality());
				renderer().draw_rect(ScreenRect{left + 1, _screen_bottom, _condition_bar_width - 2, health_bar_height - 1, ScreenRect::Origin::lower_left}, Color::red(), true);
				left += _condition_bar_width;
				// Mana
				int mana_bar_height = static_cast<int>(_condition_bar_height * player_being->mana() / player_being->spirit());
				renderer().draw_rect(ScreenRect{left + 1, _screen_bottom, _condition_bar_width - 2, mana_bar_height - 1, ScreenRect::Origin::lower_left}, Color::blue(), true);
				left += _condition_bar_width;
			}

			// Draw the body.
			{
				BodyTexturer texturer;
				texturer.visit(player_being->body());
				Texture::ptr texture = texturer.texture();
				texture->draw(ScreenPoint{0, _screen_bottom - _condition_bar_height}, HAlign::left, VAlign::bottom);
			}

			// Draw hotbar items.
			{
				ItemTexturer texturer;
				for (int i = 0; i < hud.hotbar_size; ++i) {
					int x = _hotbar_x_start + (_hotbar_slot_width + _hotbar_interslot_gap) * i;
					int y = _screen_bottom - _hotbar_bottom_gap;
					_hotbar_slot_texture->draw(ScreenPoint{x, y}, HAlign::left, VAlign::bottom);
					if (auto inv_coords = hud.hotbar[i]) {
						Item* item = player_being->inventory()[*inv_coords];
						if (item != nullptr) { /// @todo Find a way to sync the hotbar with the inventory so these checks aren't necessary.
							item->accept(texturer);
							Texture::ptr texture = texturer.texture();
							texture->draw(ScreenPoint{x + _hotbar_slot_h_padding, y - _hotbar_slot_v_padding}, HAlign::left, VAlign::bottom);
						}
					}
				}
			}

			// Draw the inventory if it's open.

			if (hud.inv_open) {
				renderer().draw_rect(ScreenRect{_inv_left, _inv_top, _inv_width, _inv_height}, Color::black(), Color::gray());
				ItemTexturer texturer;
				const auto& items = player_being->inventory().page(hud.inv_page).items;
				for (int row = 0; row < Inventory::Page::rows; ++row) {
					for (int column = 0; column < Inventory::Page::columns; ++column) {
						if (const auto& item = items[row][column]) {
							item->accept(texturer);
							Texture::ptr texture = texturer.texture();
							texture->draw(ScreenPoint{_inv_left + column * HUDView::item_icon_width, _inv_top + row * HUDView::item_icon_height});
						} else {
							renderer().draw_rect(ScreenRect{_inv_left + column * HUDView::item_icon_width, _inv_top + row * HUDView::item_icon_height, HUDView::item_icon_width, HUDView::item_icon_height}, Color::black(), Color::gray());
						}
					}
				}
			}
		}
	}

	void HUDView::load_textures_and_layout()
	{
		_hotbar_slot_texture = std::make_unique<Texture>("resources/textures/ui/hud/hotbar-slot.png", renderer());

		int window_width = _window.width();
		int window_height = _window.height();
		_screen_bottom = window_height - 1;

		_hotbar_width = _hotbar_slot_texture->width() * HUDModel::hotbar_size + _hotbar_interslot_gap * (HUDModel::hotbar_size - 1);
		_hotbar_x_start = (window_width - _hotbar_width) / 2;

		_inv_width = static_cast<int>(HUDView::inv_width_percent * window_width);
		_inv_height = static_cast<int>(HUDView::inv_height_percent * window_height);

		_inv_left = (window_width - _inv_width) / 2;
		_inv_top = (window_height - _inv_height) / 2;

		_inv_column_count = _inv_width / HUDView::item_icon_width;
	}
}