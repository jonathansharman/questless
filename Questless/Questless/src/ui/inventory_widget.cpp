//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "inventory_widget.hpp"

namespace ql {
	using namespace view::literals;

	namespace {
		constexpr view::vector item_icon_size{55.0_px, 55.0_px};
	}

	inventory_widget::inventory_widget(widget& parent, inventory& inventory) : widget{&parent}, _inv{inventory} {
		//! @todo Support item sorting (alphabetical or custom).
		//! @todo Cache the displayed items (only need to regenerate list if a turn has been taken).
	}

	view::vector inventory_widget::get_size() const {
		constexpr float width_pct = 0.7f;
		constexpr float height_pct = 0.7f;

		auto const size = parent()->get_size();
		size[0] *= width_pct;
		size[1] *= height_pct;
		return size;
	}

	void inventory_widget::update(sec elapsed_time, std::vector<sf::Event>& events) {
		auto const layout = get_bounding_box();

		for (auto event : events) {
			switch (event.type) {
				case sf::Event::Resized:
					// Update the layout when the window resizes.
					_inv_row_count = lround(height(layout) / item_icon_size[1]);
					_inv_column_count = lround(width(layout) / item_icon_size[0]);
					break;
				default:
					break;
			}
		}

		auto const mouse_pos = view::point_from_sfml(sf::Mouse::getPosition());
		int row = (mouse_pos[1] - top(layout) / item_icon_size[1];
		int column = (sf::Mouse::getPosition().x - left(layout) / item_icon_size[0];
		int index = row * _inv_column_count + column;

		bool row_in_bounds = top(layout) <= mouse_pos[1] && row < _inv_row_count;
		bool column_in_bounds = left(layout) <= mouse_pos[0] && column < _inv_column_count;
		bool index_in_bounds = 0 <= index && index < static_cast<int>(_displayed_items.size());

		if (row_in_bounds && column_in_bounds && index_in_bounds) {
			for (std::size_t key_index = 0; key_index < 10; ++key_index) {
				if (im.pressed(input_manager::index_to_num_key(key_index))) {
					_hotbar[key_index] = _displayed_items[index];
				}
			}
		}
	}

	void inventory_widget::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		auto const layout = get_bounding_box();

		// Draw background.
		sf::RectangleShape background{to_sfml(layout.size)};
		background.setOutlineColor(sf::Color::Black);
		background.setOutlineThickness(1.0f);
		background.setFillColor(sf::Color{128, 128, 128});
		background.setPosition(to_sfml(layout.position));
		target.draw(background, states);

		{ // Draw selection.
			auto const mouse_pos = view::point_from_sfml(sf::Mouse::getPosition());
			int const row = (mouse_pos[1] - top(layout)) / item_icon_size[1];
			int const column = (mouse_pos[0] - left(layout)) / item_icon_size[0];
			if (_inv_layout.top <= y_mouse && row < _inv_row_count && _inv_layout.left <= x_mouse && column < _inv_column_count) {
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
