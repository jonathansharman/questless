//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "ui/HUD.h"
#include "Game.h"
#include "entities/beings/Being.h"
#include "animation/ItemRenderer.h"
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
			if (Being const* player_being = game().beings.cptr(*_player_being_id)) {
				update_displayed_items(*player_being);

				if (input().presses(SDLK_ESCAPE)) {
					_inv_open = false;
				} else if (input().presses(SDLK_TAB)) {
					_inv_open = !_inv_open;
				}

				if (_inv_open) {
					int row = (input().y_mouse() - top(_inv_layout)) / _item_icon_size.y();
					int column = (input().x_mouse() - left(_inv_layout)) / _item_icon_size.x();
					int index = row * _inv_column_count + column;

					bool row_in_bounds = top(_inv_layout) <= input().y_mouse() && row < _inv_row_count;
					bool column_in_bounds = left(_inv_layout) <= input().x_mouse() && column < _inv_column_count;
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

		if (Being const* player_being = game().beings.cptr(player_being_id)) {
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
		if (Being const* player_being = game().beings.cptr(*_player_being_id)) {
			{ // Draw the condition bars.
				int left = 0;
				renderer().draw_box
					( ScreenSpace::Box
						{ ScreenSpace::Point{left, _screen_bottom}
						, ScreenSpace::Vector{_condition_bar_width * _conditions_count, _condition_bar_height}
						, { ScreenSpace::align_left, ScreenSpace::align_bottom }
						}
					, colors::black()
					, sdl::Fill::solid
					);
				// Health
				int health_bar_height = lround(_condition_bar_height * player_being->health / player_being->stats.vitality);
				renderer().draw_box
					( ScreenSpace::Box
						{ ScreenSpace::Point{left + 1, _screen_bottom}
						, ScreenSpace::Vector{_condition_bar_width - 2, health_bar_height - 1}
						, { ScreenSpace::align_left, ScreenSpace::align_bottom }
						}
					, colors::red()
					, sdl::Fill::solid
					);
				left += _condition_bar_width;
				// Mana
				int mana_bar_height = lround(_condition_bar_height * player_being->mana / player_being->stats.spirit);
				renderer().draw_box
					( ScreenSpace::Box
						{ ScreenSpace::Point{left + 1, _screen_bottom}
						, ScreenSpace::Vector{_condition_bar_width - 2, mana_bar_height - 1}
						, { ScreenSpace::align_left, ScreenSpace::align_bottom }
						}
					, colors::blue()
					, sdl::Fill::solid
					);
				left += _condition_bar_width;
				// Energy
				int energy_bar_height = lround(_condition_bar_height * player_being->energy / player_being->stats.stamina);
				renderer().draw_box
					( ScreenSpace::Box
						{ ScreenSpace::Point{left + 1, _screen_bottom}
						, ScreenSpace::Vector{_condition_bar_width - 2, energy_bar_height - 1}
						, { ScreenSpace::align_left, ScreenSpace::align_bottom }
						}
					, colors::cyan()
					, sdl::Fill::solid
					);
				left += _condition_bar_width;
				// Satiety
				int satiety_bar_height = lround(_condition_bar_height * player_being->satiety / Being::max_satiety);
				renderer().draw_box
					( ScreenSpace::Box
						{ ScreenSpace::Point{left + 1, _screen_bottom}
						, ScreenSpace::Vector{_condition_bar_width - 2, satiety_bar_height - 1}
						, { ScreenSpace::align_left, ScreenSpace::align_bottom }
						}
					, colors::brown()
					, sdl::Fill::solid
					);
				left += _condition_bar_width;
				// Alertness
				int alertness_bar_height = lround(_condition_bar_height * player_being->alertness / Being::max_alertness);
				renderer().draw_box
					( ScreenSpace::Box
						{ ScreenSpace::Point{left + 1, _screen_bottom}
						, ScreenSpace::Vector{_condition_bar_width - 2, alertness_bar_height - 1}
						, { ScreenSpace::align_left, ScreenSpace::align_bottom }
						}
					, colors::yellow()
					, sdl::Fill::solid
					);
				left += _condition_bar_width;
			}

			{ // Draw the body.
				BodyTexturer texturer;
				texturer.visit(player_being->body);
				uptr<Texture> texture = texturer.texture();
				texture->draw(ScreenSpace::Point{0, _screen_bottom - _condition_bar_height}, TextureSpace::align_left, TextureSpace::align_bottom);
			}

			{ // Draw hotbar items.
				for (int i = 0; i < _hotbar_size; ++i) {
					int x = _hotbar_x_start + (_hotbar_slot_width + _hotbar_interslot_gap) * i;
					int y = _screen_bottom - _hotbar_bottom_gap;
					_hotbar_slot_texture->draw(ScreenSpace::Point{x, y}, TextureSpace::align_left, TextureSpace::align_bottom);
					if (std::optional<Id<Item>> opt_item_id = _hotbar[i]) {
						Item const& item = game().items.cref(*opt_item_id);
						ItemRenderer item_renderer{ScreenSpace::Point{x + _hotbar_slot_h_padding, y - _hotbar_slot_v_padding - 55}}; //! @todo Remove magic number or add alignment back in. 55 is the height of item textures.
						item.accept(item_renderer);
					}
				}
			}

			// Draw the inventory if it's open.
			if (_inv_open) {
				renderer().draw_box(_inv_layout, colors::black(), colors::gray());

				{ // Draw selection.
					int x_mouse = input().x_mouse();
					int y_mouse = input().y_mouse();
					int row = (y_mouse - top(_inv_layout)) / _item_icon_size.y();
					int column = (x_mouse - left(_inv_layout)) / _item_icon_size.x();
					if (top(_inv_layout) <= y_mouse && row < _inv_row_count && left(_inv_layout) <= x_mouse && column < _inv_column_count) {
						renderer().draw_box
							( ScreenSpace::Box
								{ ScreenSpace::Point{left(_inv_layout) + _item_icon_size.x() * column, top(_inv_layout) + _item_icon_size.y() * row}
								, _item_icon_size
								}
							, colors::black()
							, colors::silver()
							);
					}
				}

				// Draw item icons.
				for (size_t i = 0; i < _displayed_items.size(); ++i) {
					int row = i / _inv_column_count;
					int column = i % _inv_column_count;
					ItemRenderer texturer{ScreenSpace::Point{left(_inv_layout) + column * _item_icon_size.x(), top(_inv_layout) + row * _item_icon_size.y()}};
					_displayed_items[i].get().accept(texturer);
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

		// Calculate number of visible inventory rows and columns.
		_inv_row_count = std::max(1l, lround(_inv_height_percent * window().height() / _item_icon_size.y()));
		_inv_column_count = std::max(1l, lround(_inv_width_percent * window().width() / _item_icon_size.x()));

		// Set inventory layout.
		_inv_layout = ScreenSpace::Box
			{ window().center()
			, ScreenSpace::Vector{_inv_column_count * _item_icon_size.x(), _inv_row_count * _item_icon_size.y()}
			, { ScreenSpace::align_center, ScreenSpace::align_middle }
			};
	}

	void HUD::update_displayed_items(Being const& player_being)
	{
		_displayed_items = {player_being.inventory.items.begin(), player_being.inventory.items.end()};
		// Sort displayed items alphabetically.
		//! @todo Custom displayed items order and cache the displayed items (only need to regenerate list if a turn has been taken).
		std::sort(_displayed_items.begin(), _displayed_items.end(), [](Item const& left, Item const& right) {
			return left.name() < right.name();
		});
	}
}
