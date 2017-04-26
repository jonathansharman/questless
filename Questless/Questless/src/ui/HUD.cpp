//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "ui/HUD.h"
#include "Game.h"
#include "entities/beings/Being.h"
#include "animation/ItemTexturer.h"
#include "animation/BodyTexturer.h"

using namespace sdl;
using namespace units;

namespace questless
{
	HUD::HUD()
	{
		_fnt_item_count = std::make_unique<Font>("resources/fonts/dumbledor1.ttf", _item_count_font_size, SDL_BLENDMODE_BLEND);

		load_textures_and_layout();
	}
	
	void HUD::update()
	{
		if (_player_being_id) {
			if (Being* player_being = game().beings.get(*_player_being_id)) {
				update_displayed_items(*player_being);

				if (input().presses(SDLK_ESCAPE)) {
					_inv_open = false;
				} else if (input().presses(SDLK_TAB)) {
					_inv_open = !_inv_open;
				}

				if (_inv_open) {
					int row = (input().y_mouse() - _inv_top) / _item_icon_height;
					int column = (input().x_mouse() - _inv_left) / _item_icon_width;
					int index = row * _inv_column_count + column;

					bool row_in_bounds = _inv_top <= input().y_mouse() && row < _inv_row_count;
					bool column_in_bounds = _inv_left <= input().x_mouse() && column < _inv_column_count;
					bool index_in_bounds = 0 <= index && index < static_cast<int>(_displayed_items.size());

					if (row_in_bounds && column_in_bounds && index_in_bounds) {
						if (input().presses(SDLK_1)) {
							_hotbar[0] = _displayed_items[index].get().id;
						}
						if (input().presses(SDLK_2)) {
							_hotbar[1] = _displayed_items[index].get().id;
						}
						if (input().presses(SDLK_3)) {
							_hotbar[2] = _displayed_items[index].get().id;
						}
						if (input().presses(SDLK_4)) {
							_hotbar[3] = _displayed_items[index].get().id;
						}
						if (input().presses(SDLK_5)) {
							_hotbar[4] = _displayed_items[index].get().id;
						}
						if (input().presses(SDLK_6)) {
							_hotbar[5] = _displayed_items[index].get().id;
						}
						if (input().presses(SDLK_7)) {
							_hotbar[6] = _displayed_items[index].get().id;
						}
						if (input().presses(SDLK_8)) {
							_hotbar[7] = _displayed_items[index].get().id;
						}
						if (input().presses(SDLK_9)) {
							_hotbar[8] = _displayed_items[index].get().id;
						}
						if (input().presses(SDLK_0)) {
							_hotbar[9] = _displayed_items[index].get().id;
						}
					}
				}
			}
		}
	}

	void HUD::set_player_being_id(Id<Being> player_being_id)
	{
		_player_being_id = player_being_id;
		_inv_page = 0;

		if (Being* player_being = game().beings.get(player_being_id)) {
			update_displayed_items(*player_being);

			// Fill the hotbar with as many items as possible.
			size_t count = 0;
			for (Item const& item : _displayed_items) {
				_hotbar[count++] = item.id;
				if (count == _hotbar_size) {
					return;
				}
			}
		} else {
			for (size_t i = 0; i < _hotbar_size; ++i) {
				_hotbar[i] = std::nullopt;
			}
		}
	}

	void HUD::draw()
	{
		if (!_player_being_id) {
			return;
		}
		if (Being* player_being = game().beings.get(*_player_being_id)) {
			{ // Draw the condition bars.
				int left = 0;
				renderer().draw_rect(ScreenRect{left, _screen_bottom, _condition_bar_width * _conditions_count, _condition_bar_height, ScreenRect::Origin::lower_left}, Color::black(), true);
				// Health
				int health_bar_height = lround(_condition_bar_height * player_being->health / player_being->stats.vitality);
				renderer().draw_rect(ScreenRect{left + 1, _screen_bottom, _condition_bar_width - 2, health_bar_height - 1, ScreenRect::Origin::lower_left}, Color::red(), true);
				left += _condition_bar_width;
				// Mana
				int mana_bar_height = lround(_condition_bar_height * player_being->mana / player_being->stats.spirit);
				renderer().draw_rect(ScreenRect{left + 1, _screen_bottom, _condition_bar_width - 2, mana_bar_height - 1, ScreenRect::Origin::lower_left}, Color::blue(), true);
				left += _condition_bar_width;
				// Energy
				int energy_bar_height = lround(_condition_bar_height * player_being->energy / player_being->stats.stamina);
				renderer().draw_rect(ScreenRect{left + 1, _screen_bottom, _condition_bar_width - 2, energy_bar_height - 1, ScreenRect::Origin::lower_left}, Color::cyan(), true);
				left += _condition_bar_width;
				// Satiety
				int satiety_bar_height = lround(_condition_bar_height * player_being->satiety / Being::max_satiety);
				renderer().draw_rect(ScreenRect{left + 1, _screen_bottom, _condition_bar_width - 2, satiety_bar_height - 1, ScreenRect::Origin::lower_left}, Color::brown(), true);
				left += _condition_bar_width;
				// Alertness
				int alertness_bar_height = lround(_condition_bar_height * player_being->alertness / Being::max_alertness);
				renderer().draw_rect(ScreenRect{left + 1, _screen_bottom, _condition_bar_width - 2, alertness_bar_height - 1, ScreenRect::Origin::lower_left}, Color::yellow(), true);
				left += _condition_bar_width;
			}

			{ // Draw the body.
				BodyTexturer texturer;
				texturer.visit(player_being->body);
				uptr<Texture> texture = texturer.texture();
				texture->draw(ScreenPoint{0, _screen_bottom - _condition_bar_height}, HAlign::left, VAlign::bottom);
			}

			{ // Draw hotbar items.
				ItemTexturer texturer;
				for (int i = 0; i < _hotbar_size; ++i) {
					int x = _hotbar_x_start + (_hotbar_slot_width + _hotbar_interslot_gap) * i;
					int y = _screen_bottom - _hotbar_bottom_gap;
					_hotbar_slot_texture->draw(ScreenPoint{x, y}, HAlign::left, VAlign::bottom);
					if (std::optional<Id<Item>> opt_item_id = _hotbar[i]) {
						Item& item = game().items.get_ref(*opt_item_id);
						item.accept(texturer);
						uptr<Texture> texture = texturer.texture();
						texture->draw(ScreenPoint{x + _hotbar_slot_h_padding, y - _hotbar_slot_v_padding}, HAlign::left, VAlign::bottom);
					}
				}
			}

			// Draw the inventory if it's open.
			if (_inv_open) {
				renderer().draw_rect(ScreenRect{_inv_left, _inv_top, _inv_width, _inv_height}, Color::black(), Color::gray());
				ItemTexturer texturer;

				{ // Draw selection.
					int x_mouse = input().x_mouse();
					int y_mouse = input().y_mouse();
					int row = (y_mouse - _inv_top) / _item_icon_height;
					int column = (x_mouse - _inv_left) / _item_icon_width;
					if (_inv_top <= y_mouse && row < _inv_row_count && _inv_left <= x_mouse && column < _inv_column_count) {
						int x = _inv_left + _item_icon_width * column;
						int y = _inv_top + _item_icon_height * row;
						renderer().draw_rect(ScreenRect{x, y, _item_icon_width, _item_icon_height}, Color::black(), Color::silver());
					}
				}

				{ // Draw item icons.
					int index = 0;
					for (Item const& item : _displayed_items) {
						item.accept(texturer);
						uptr<Texture> texture = texturer.texture();
						int row = index / _inv_column_count;
						int column = index % _inv_column_count;
						texture->draw(ScreenPoint{ _inv_left + column * _item_icon_width, _inv_top + row * _item_icon_height });
						++index;
					}
				}
			}
		}
	}

	void HUD::load_textures_and_layout()
	{
		_screen_bottom = window().height() - 1;

		// Load hotbar textures and position hotbar.
		_hotbar_slot_texture = std::make_unique<Texture>("resources/textures/ui/hud/hotbar-slot.png");
		_hotbar_width = _hotbar_slot_texture->width() * _hotbar_size + _hotbar_interslot_gap * (_hotbar_size - 1);
		_hotbar_x_start = (window().width() - _hotbar_width) / 2;

		// Calculate inventory dimensions and number of visible rows and columns.
		_inv_height = static_cast<int>(_inv_height_percent * window().height());
		_inv_width = static_cast<int>(_inv_width_percent * window().width());
		_inv_row_count = std::max(1, _inv_height / _item_icon_height);
		_inv_column_count = std::max(1, _inv_width / _item_icon_width);

		// Adjust inventory dimensions so they're multiples of the item icon dimensions.
		_inv_height = _inv_row_count * _item_icon_height;
		_inv_width = _inv_column_count * _item_icon_width;

		// Position inventory.
		_inv_left = (window().width() - _inv_width) / 2;
		_inv_top = (window().height() - _inv_height) / 2;
	}

	void HUD::update_displayed_items(Being& player_being)
	{
		_displayed_items = {player_being.inventory.items().begin(), player_being.inventory.items().end()};
		// Sort displayed items alphabetically.
		//! @todo Custom displayed items order and cache the displayed items (only need to regenerate list if a turn has been taken).
		std::sort(_displayed_items.begin(), _displayed_items.end(), [](Item const& left, Item const& right) {
			return left.name() < right.name();
		});
	}
}
