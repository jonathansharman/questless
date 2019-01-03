//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "hud.hpp"

#include "game.hpp"
#include "entities/beings/being.hpp"
#include "animation/item_renderer.hpp"
#include "animation/body_texturer.hpp"

using namespace sdl;
using namespace units;

namespace ql {
	namespace {
		constexpr spaces::window::vector item_icon_size{55, 55};
	}
	
	void hud::update() {
		if (_player_being_id) {
			if (being const* player_being = the_game().beings.cptr(*_player_being_id)) {
				update_displayed_items(*player_being);

				if (the_input().presses(SDLK_ESCAPE)) {
					_inv_open = false;
				} else if (the_input().presses(SDLK_TAB)) {
					_inv_open = !_inv_open;
				}

				if (_inv_open) {
					int row = (the_input().y_mouse() - top(_inv_layout)) / item_icon_size.y();
					int column = (the_input().x_mouse() - left(_inv_layout)) / item_icon_size.x();
					int index = row * _inv_column_count + column;

					bool row_in_bounds = top(_inv_layout) <= the_input().y_mouse() && row < _inv_row_count;
					bool column_in_bounds = left(_inv_layout) <= the_input().x_mouse() && column < _inv_column_count;
					bool index_in_bounds = 0 <= index && index < static_cast<int>(_displayed_items.size());

					if (row_in_bounds && column_in_bounds && index_in_bounds) {
						if (the_input().presses(SDLK_1)) {
							_hotbar[0] = _displayed_items[index].get().id;
						}
						if (the_input().presses(SDLK_2)) {
							_hotbar[1] = _displayed_items[index].get().id;
						}
						if (the_input().presses(SDLK_3)) {
							_hotbar[2] = _displayed_items[index].get().id;
						}
						if (the_input().presses(SDLK_4)) {
							_hotbar[3] = _displayed_items[index].get().id;
						}
						if (the_input().presses(SDLK_5)) {
							_hotbar[4] = _displayed_items[index].get().id;
						}
						if (the_input().presses(SDLK_6)) {
							_hotbar[5] = _displayed_items[index].get().id;
						}
						if (the_input().presses(SDLK_7)) {
							_hotbar[6] = _displayed_items[index].get().id;
						}
						if (the_input().presses(SDLK_8)) {
							_hotbar[7] = _displayed_items[index].get().id;
						}
						if (the_input().presses(SDLK_9)) {
							_hotbar[8] = _displayed_items[index].get().id;
						}
						if (the_input().presses(SDLK_0)) {
							_hotbar[9] = _displayed_items[index].get().id;
						}
					}
				}
			}
		}
	}

	void hud::set_player_being_id(id<being> player_being_id) {
		_player_being_id = player_being_id;
		_inv_page = 0;

		if (being const* player_being = the_game().beings.cptr(player_being_id)) {
			update_displayed_items(*player_being);

			// Fill the hotbar with as many items as possible.
			size_t count = 0;
			for (item const& item : _displayed_items) {
				_hotbar[count++] = item.id;
				if (count == _hotbar_size) {
					return;
				}
			}
		} else {
			std::fill(_hotbar.begin(), _hotbar.end(), std::nullopt);
		}
	}

	void hud::draw() {
		constexpr int item_count_font_size = 10;
		static auto fnt_item_count = umake<font>("resources/fonts/dumbledor1.ttf", item_count_font_size, SDL_BLENDMODE_BLEND);

		if (!_player_being_id) {
			return;
		}
		if (being const* player_being = the_game().beings.cptr(*_player_being_id)) {
			constexpr int conditions_count = 2;
			constexpr int condition_bar_width = 10;
			constexpr int condition_bar_height = 100;

			constexpr int hotbar_interslot_gap = 2;
			constexpr int hotbar_bottom_gap = 2;
			constexpr int hotbar_slot_width = 65;
			constexpr int hotbar_slot_h_padding = 5;
			constexpr int hotbar_slot_v_padding = 5;

			// Draw condition bars.
			int const screen_bottom = the_window().height() - 1;
			the_renderer().draw_box
				( spaces::window::box
					{ spaces::window::point{0, screen_bottom}
					, spaces::window::vector{condition_bar_width * conditions_count, condition_bar_height}
					, { spaces::window::align_left, spaces::window::align_bottom }
					}
				, colors::black()
				);
			auto draw_bar = [condition_bar_width, condition_bar_height, screen_bottom, left = 0](double ratio, colors::color const& color) mutable {
				the_renderer().draw_box
					( spaces::window::box
						{ spaces::window::point{left + 1, screen_bottom}
						, spaces::window::vector{condition_bar_width - 2, lround(condition_bar_height * ratio) - 1}
						, { spaces::window::align_left, spaces::window::align_bottom }
						}
					, color
					);
				left += condition_bar_width;
			};
			// Blood
			draw_bar((player_being->body.blood.value() / player_being->body.total_vitality() / body_part::blood_per_vitality).value, colors::red());
			// Energy
			draw_bar((player_being->energy.value() / player_being->stats.a.stamina.value()).value, colors::cyan());
			// Satiety
			draw_bar((player_being->satiety.value() / being::max_satiety).value, colors::brown());
			// Alertness
			draw_bar((player_being->alertness.value() / being::max_alertness).value, colors::yellow());

			{ // Draw the body.
				body_texturer texturer;
				texturer.visit(player_being->body);
				uptr<texture> texture = texturer.texture();
				texture->draw(spaces::window::point{0, screen_bottom - condition_bar_height}, spaces::window::align_left, spaces::window::align_bottom);
			}

			{ // Draw hotbar items.
				static auto const hotbar_slot_texture = umake<texture>("resources/textures/ui/hud/hotbar-slot.png");

				int const hotbar_width = hotbar_slot_texture->width() * _hotbar_size + hotbar_interslot_gap * (_hotbar_size - 1);
				int const hotbar_x_start = (the_window().width() - hotbar_width) / 2;

				for (int i = 0; i < _hotbar_size; ++i) {
					int x = hotbar_x_start + (hotbar_slot_width + hotbar_interslot_gap) * i;
					int y = screen_bottom - hotbar_bottom_gap;
					hotbar_slot_texture->draw(spaces::window::point{x, y}, spaces::window::align_left, spaces::window::align_bottom);
					if (std::optional<id<item>> opt_item_id = _hotbar[i]) {
						item const& item = the_game().items.cref(*opt_item_id);
						item_renderer item_renderer{spaces::window::point{x + hotbar_slot_h_padding, y - hotbar_slot_v_padding - 55}}; //! @todo Remove magic number or add alignment back in. 55 is the height of item textures.
						item.accept(item_renderer);
					}
				}
			}

			// Draw the inventory if it's open.
			if (_inv_open) {
				static constexpr float inv_width_pct = 0.7f;
				static constexpr float inv_height_pct = 0.7f;

				// Update the inventory layout if the window was resized.
				if (the_input().window_resized())
					// Calculate number of visible inventory rows and columns.
					_inv_row_count = std::max(1l, lround(inv_height_pct * the_window().height() / item_icon_size.y()));
					_inv_column_count = std::max(1l, lround(inv_width_pct * the_window().width() / item_icon_size.x()));

					_inv_layout = spaces::window::box
						{ the_window().window_center()
						, spaces::window::vector{_inv_column_count * item_icon_size.x(), _inv_row_count * item_icon_size.y()}
						, {spaces::window::align_center, spaces::window::align_middle}
						};

				the_renderer().draw_box(_inv_layout, 1, colors::black(), colors::gray());

				{ // Draw selection.
					int const x_mouse = the_input().x_mouse();
					int const y_mouse = the_input().y_mouse();
					int const row = (y_mouse - top(_inv_layout)) / item_icon_size.y();
					int const column = (x_mouse - left(_inv_layout)) / item_icon_size.x();
					if (top(_inv_layout) <= y_mouse && row < _inv_row_count && left(_inv_layout) <= x_mouse && column < _inv_column_count) {
						the_renderer().draw_box
							( spaces::window::box
								{ spaces::window::point{left(_inv_layout) + item_icon_size.x() * column, top(_inv_layout) + item_icon_size.y() * row}
								, item_icon_size
								}
							, 1
							, colors::black()
							, colors::silver()
							);
					}
				}

				// Draw item icons.
				for (size_t i = 0; i < _displayed_items.size(); ++i) {
					int row = i / _inv_column_count;
					int column = i % _inv_column_count;
					item_renderer texturer{spaces::window::point{left(_inv_layout) + column * item_icon_size.x(), top(_inv_layout) + row * item_icon_size.y()}};
					_displayed_items[i].get().accept(texturer);
				}
			}
		}
	}

	void hud::update_displayed_items(being const& player_being) {
		_displayed_items = {player_being.inventory.items.begin(), player_being.inventory.items.end()};
		// Sort displayed items alphabetically.
		//! @todo Custom displayed items order and cache the displayed items (only need to regenerate list if a turn has been taken).
		std::sort(_displayed_items.begin(), _displayed_items.end(), [](item const& left, item const& right) {
			return left.name() < right.name();
		});
	}
}
