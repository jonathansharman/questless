//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "inventory_widget.hpp"

#include "hotbar.hpp"

namespace ql {
	using namespace view::literals;

	namespace {
		constexpr view::vector item_icon_size{55.0_px, 55.0_px};
	}

	inventory_widget::inventory_widget(inventory& inventory, hotbar& hotbar)
		: _inv{inventory}
		, _hotbar{hotbar} //
	{
		//! @todo Support item sorting (alphabetical or custom).
		//! @todo Cache the displayed items (only need to regenerate list if a turn has been taken).
	}

	view::vector inventory_widget::get_size() const {
		return _size;
	}

	void inventory_widget::update(sec /*elapsed_time*/) {}

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
			float const row = floor((mouse_pos[1] - top(layout)) / item_icon_size[1]);
			float const col = floor((mouse_pos[0] - left(layout)) / item_icon_size[0]);
			if (0.0f < row && row < _row_count && 0.0f < col && col < _col_count) {
				sf::RectangleShape selection_box{to_sfml(item_icon_size)};
				auto const pos = layout.position + view::vector{item_icon_size[0] * col, item_icon_size[1] * row};
				selection_box.setPosition(to_sfml(pos));
				selection_box.setOutlineColor(sf::Color::Black);
				selection_box.setOutlineThickness(1.0f);
				selection_box.setFillColor(sf::Color{192, 192, 192});
				target.draw(selection_box, states);
			}
		}

		// Draw item icons.

		//! @todo Draw item icons.
	}

	auto inventory_widget::set_position(view::point position) -> void {
		_position = position;
	}

	auto inventory_widget::get_position() const -> view::point {
		return _position;
	}

	auto inventory_widget::on_parent_resize(view::vector parent_size) -> void {
		constexpr float width_pct = 0.7f;
		constexpr float height_pct = 0.7f;

		_size = parent_size;
		_size[0] *= width_pct;
		_size[1] *= height_pct;

		_row_count = lround(_size[1] / item_icon_size[1]);
		_col_count = lround(_size[0] / item_icon_size[0]);
	};

	widget::event_handled inventory_widget::on_key_press(sf::Event::KeyEvent const& event) {
		switch (event.code) {
			case sf::Keyboard::Num1:
				assign_idx(0);
				break;
			case sf::Keyboard::Num2:
				assign_idx(1);
				break;
			case sf::Keyboard::Num3:
				assign_idx(2);
				break;
			case sf::Keyboard::Num4:
				assign_idx(3);
				break;
			case sf::Keyboard::Num5:
				assign_idx(4);
				break;
			case sf::Keyboard::Num6:
				assign_idx(5);
				break;
			case sf::Keyboard::Num7:
				assign_idx(6);
				break;
			case sf::Keyboard::Num8:
				assign_idx(7);
				break;
			case sf::Keyboard::Num9:
				assign_idx(8);
				break;
			case sf::Keyboard::Num0:
				assign_idx(9);
				break;
			default:
				return event_handled::no;
		}
		return event_handled::yes;
	}

	void inventory_widget::assign_idx(size_t hotbar_idx) {
		auto const layout = get_bounding_box();

		auto const mouse_pos = view::point_from_sfml(sf::Mouse::getPosition());

		// Ensure row is in bounds.
		int const row = static_cast<int>(floor((mouse_pos[1] - top(layout)) / item_icon_size[1]).value);
		if (row < 0 || row >= _row_count) { return; }
		// Ensure column is in bounds.
		int const col = static_cast<int>(floor((mouse_pos[0] - left(layout)) / item_icon_size[0]).value);
		if (col < 0 || col >= _col_count) { return; }
		// Ensure index is in bounds.
		size_t const idx = row * _col_count + col;
		if (idx >= _displayed_items.size()) { return; }

		// Add the selected item to the hotbar.
		_hotbar.set_item(hotbar_idx, _displayed_items[idx]);
	}
}