//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "hud.hpp"

#include "animation/item_animator.hpp"
#include "entities/beings/being.hpp"
#include "game.hpp"

using namespace media;

namespace ql {
	namespace {
		sf::Vector2i const item_icon_size{55, 55};
	}

	void hud::update(sec elapsed_time, input_manager& im) {
		if (being const* player_being = the_game().beings.cptr(_player_being_id)) {
			update_displayed_items(*player_being);

			if (im.pressed(sf::Keyboard::Escape)) {
				_inv_open = false;
			} else if (im.pressed(sf::Keyboard::Tab)) {
				_inv_open = !_inv_open;
			}

			if (_inv_open) {
				int row = (sf::Mouse::getPosition().y - _inv_layout.top) / item_icon_size.y;
				int column = (sf::Mouse::getPosition().x - _inv_layout.left) / item_icon_size.x;
				int index = row * _inv_column_count + column;

				bool row_in_bounds = _inv_layout.top <= sf::Mouse::getPosition().y && row < _inv_row_count;
				bool column_in_bounds = _inv_layout.left <= sf::Mouse::getPosition().x && column < _inv_column_count;
				bool index_in_bounds = 0 <= index && index < static_cast<int>(_displayed_items.size());

				if (row_in_bounds && column_in_bounds && index_in_bounds) {
					for (std::size_t key_index = 0; key_index < 10; ++key_index) {
						if (im.pressed(input_manager::index_to_num_key(key_index))) {
							_hotbar[key_index] = _displayed_items[index].get().id;
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
				if (count == _hotbar_size) { return; }
			}
		} else {
			std::fill(_hotbar.begin(), _hotbar.end(), std::nullopt);
		}
	}

	void hud::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		//! @todo This should not be static.
		static auto fnt_item_count = umake<sf::Font>("resources/fonts/dumbledor1.ttf", 10);

		if (being const* player_being = the_game().beings.cptr(_player_being_id)) {
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
			the_renderer().draw_box(spaces::window::box{spaces::window::point{0, screen_bottom},
										spaces::window::vector{condition_bar_width * conditions_count, condition_bar_height},
										{spaces::window::align_left, spaces::window::align_bottom}},
				colors::black());
			auto draw_bar = [condition_bar_width, condition_bar_height, screen_bottom, left = 0](
								double ratio, colors::color const& color) mutable {
				the_renderer().draw_box(
					spaces::window::box{spaces::window::point{left + 1, screen_bottom},
						spaces::window::vector{condition_bar_width - 2, lround(condition_bar_height * ratio) - 1},
						{spaces::window::align_left, spaces::window::align_bottom}},
					color);
				left += condition_bar_width;
			};
			// Blood
			draw_bar(
				(player_being->body.blood.value() / player_being->body.total_vitality() / body_part::blood_per_vitality).value,
				sf::Color::Red);
			// Energy
			draw_bar((player_being->energy.value() / player_being->stats.a.stamina.value()).value, sf::Color::Cyan);
			// Satiety
			draw_bar((player_being->satiety.value() / being::max_satiety).value, sf::Color{130, 80, 40});
			// Alertness
			draw_bar((player_being->alertness.value() / being::max_alertness).value, sf::Color::Yellow);

			{ // Draw the body.
				body_renderer renderer;
				renderer.visit(player_being->body);
				uptr<texture> texture = texturer.texture();
				texture->draw(spaces::window::point{0, screen_bottom - condition_bar_height},
					spaces::window::align_left,
					spaces::window::align_bottom);
			}

			{ // Draw hotbar items.
				static auto const hotbar_slot_texture = umake<texture>("resources/textures/ui/hud/hotbar-slot.png");

				int const hotbar_width = hotbar_slot_texture->width() * _hotbar_size +
										 hotbar_interslot_gap * (_hotbar_size - 1);
				int const hotbar_x_start = (the_window().width() - hotbar_width) / 2;

				for (int i = 0; i < _hotbar_size; ++i) {
					int x = hotbar_x_start + (hotbar_slot_width + hotbar_interslot_gap) * i;
					int y = screen_bottom - hotbar_bottom_gap;
					hotbar_slot_texture->draw(
						spaces::window::point{x, y}, spaces::window::align_left, spaces::window::align_bottom);
					if (std::optional<id<item>> opt_item_id = _hotbar[i]) {
						item const& item = the_game().items.cref(*opt_item_id);
						item_animator item_animator{spaces::window::point{x + hotbar_slot_h_padding,
							y - hotbar_slot_v_padding - 55}}; //! @todo Remove magic number or add alignment back in. 55
															  //! is the height of item textures.
						item.accept(item_animator);
					}
				}
			}

			// Draw the inventory if it's open.
			if (_inv_open) {
				static constexpr float inv_width_pct = 0.7f;
				static constexpr float inv_height_pct = 0.7f;

				// Update the inventory layout if the window was resized.
				if (im.window_resized()) {
					// Calculate number of visible inventory rows and columns.
					_inv_row_count = std::max(1l, lround(inv_height_pct * the_window().getSize().y / item_icon_size.y));
					_inv_column_count = std::max(1l, lround(inv_width_pct * the_window().getSize().x / item_icon_size.x));

					_inv_layout = spaces::window::box{the_window().getSize() / 2,
						spaces::window::vector{_inv_column_count * item_icon_size.x, _inv_row_count * item_icon_size.y},
						{spaces::window::align_center, spaces::window::align_middle}};
				}

				the_renderer().draw_box(_inv_layout, 1, sf::Color::Black, sf::Color{128, 128, 128});

				{ // Draw selection.
					int const x_mouse = sf::Mouse::getPosition().x;
					int const y_mouse = sf::Mouse::getPosition().y;
					int const row = (y_mouse - _inv_layout.top) / item_icon_size.y;
					int const column = (x_mouse - _inv_layout.left) / item_icon_size.x;
					if (_inv_layout.top <= y_mouse && row < _inv_row_count && _inv_layout.left <= x_mouse &&
						column < _inv_column_count) {
						the_renderer().draw_box(
							spaces::window::box{spaces::window::point{_inv_layout.left + item_icon_size.x * column,
													_inv_layout.top + item_icon_size.y * row},
								item_icon_size},
							1,
							sf::Color::Black,
							sf::Color{192, 192, 192});
					}
				}

				// Draw item icons.
				for (size_t i = 0; i < _displayed_items.size(); ++i) {
					int row = i / _inv_column_count;
					int column = i % _inv_column_count;
					item_animator texturer{spaces::window::point{
						_inv_layout.left + column * item_icon_size.x, _inv_layout.top + row * item_icon_size.y}};
					_displayed_items[i].get().accept(texturer);
				}
			}
		}
	}

	void hud::update_displayed_items(being const& player_being) {
		_displayed_items = {player_being.inventory.items.begin(), player_being.inventory.items.end()};
		// Sort displayed items alphabetically.
		//! @todo Custom displayed items order and cache the displayed items (only need to regenerate list if a turn has
		//! been taken).
		std::sort(_displayed_items.begin(), _displayed_items.end(), [](item const& left, item const& right) {
			return left.name() < right.name();
		});
	}
}
