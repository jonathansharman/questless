//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "widget.hpp"

namespace ql {
	using namespace view::literals;

	namespace {
		constexpr view::vector item_icon_size{55.0_px, 55.0_px};
	}

	//! A UI element.
	struct inventory_widget : widget {
		inventory_widget(widget& parent) : widget{&parent} {}

		view::vector get_local_offset() const {
			// Center in parent widget.
			return parent->get_size() - get_size() / 2;
		}

		view::vector get_size() const {
			return 0.7f * parent->get_size();
		}

		void update(sec elapsed_time, std::vector<sf::Event>& events) {
			// Update the inventory layout if the window was resized.
			if (im.window_resized()) {
				// Calculate number of visible inventory rows and columns.
				_inv_row_count = std::max(1l, lround(inv_height_pct * the_window().getSize().y / item_icon_size.y));
				_inv_column_count = std::max(1l, lround(inv_width_pct * the_window().getSize().x / item_icon_size.x));

				_inv_layout = spaces::window::box{the_window().getSize() / 2,
					spaces::window::vector{_inv_column_count * item_icon_size.x, _inv_row_count * item_icon_size.y},
					{spaces::window::align_center, spaces::window::align_middle}};
			}

			int row = (sf::Mouse::getPosition().y - _inv_layout.top) / item_icon_size[1];
			int column = (sf::Mouse::getPosition().x - _inv_layout.left) / item_icon_size[0];
			int index = row * _inv_column_count + column;

			bool row_in_bounds = _inv_layout.top <= sf::Mouse::getPosition().y && row < _inv_row_count;
			bool column_in_bounds = _inv_layout.left <= sf::Mouse::getPosition().x && column < _inv_column_count;
			bool index_in_bounds = 0 <= index && index < static_cast<int>(_displayed_items.size());

			if (row_in_bounds && column_in_bounds && index_in_bounds) {
				for (std::size_t key_index = 0; key_index < 10; ++key_index) {
					if (im.pressed(input_manager::index_to_num_key(key_index))) {
						_hotbar[key_index] = _displayed_items[index];
					}
				}
			}
		}

		void draw(sf::RenderTarget& target, sf::RenderStates states) const {
			auto position = get_position();

			// Draw background.
			sf::RectangleShape background{to_sfml(get_size())};
			background.setOutlineColor(sf::Color::Black);
			background.setFillColor(sf::Color{128, 128, 128});
			background.setPosition(to_sfml(position));
			target.draw(background, states);

			{ // Draw selection.
				auto const mouse_pos = sf::Mouse::getPosition();
				int const row = (mouse_pos.y - _inv_layout.top) / item_icon_size.y;
				int const column = (x_mouse - _inv_layout.left) / item_icon_size.x;
				if (_inv_layout.top <= y_mouse && row < _inv_row_count && _inv_layout.left <= x_mouse &&
					column < _inv_column_count) {
					the_renderer().draw_box(spaces::window::box{spaces::window::point{_inv_layout.left + item_icon_size.x * column,
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
