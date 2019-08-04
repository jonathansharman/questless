//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "hotbar.hpp"

namespace ql {
	using namespace view::literals;

	auto hotbar::get_size() const -> view::vector {
		return {static_cast<float>(_item_widgets.size()) * item_widget::size[0], item_widget::size[1]};
	}

	auto hotbar::update(sec /*elapsed_time*/) -> void {}

	auto hotbar::draw(sf::RenderTarget& target, sf::RenderStates states) const -> void {
		for (auto const& item_widget : _item_widgets) {
			target.draw(item_widget, states);
		}
	}

	auto hotbar::set_position(view::point position) -> void {
		_position = position;
		for (size_t i = 0; i < _item_widgets.size(); ++i) {
			view::vector offset{static_cast<float>(i) * item_widget::size[0], item_widget::size[1]};
			_item_widgets[i].set_position(_position + offset);
		}
	}

	auto hotbar::get_position() const -> view::point {
		return _position;
	}

	auto hotbar::on_key_press(sf::Event::KeyEvent const& event) -> event_handled {
		switch (event.code) {
			case sf::Keyboard::Space:
				click(_most_recent_idx);
				return event_handled::yes;
			case sf::Keyboard::Num1:
				click(0);
				return event_handled::yes;
			case sf::Keyboard::Num2:
				click(1);
				return event_handled::yes;
			case sf::Keyboard::Num3:
				click(2);
				return event_handled::yes;
			case sf::Keyboard::Num4:
				click(3);
				return event_handled::yes;
			case sf::Keyboard::Num5:
				click(4);
				return event_handled::yes;
			case sf::Keyboard::Num6:
				click(5);
				return event_handled::yes;
			case sf::Keyboard::Num7:
				click(6);
				return event_handled::yes;
			case sf::Keyboard::Num8:
				click(7);
				return event_handled::yes;
			case sf::Keyboard::Num9:
				click(8);
				return event_handled::yes;
			case sf::Keyboard::Num0:
				click(9);
				return event_handled::yes;
			default:
				return event_handled::no;
		}
	}

	auto hotbar::on_mouse_press(sf::Event::MouseButtonEvent const& event) -> event_handled {
		for (auto& item_widget : _item_widgets) {
			if (item_widget.on_mouse_press(event) == event_handled::yes) { return event_handled::yes; }
		}
		return event_handled::no;
	}

	auto hotbar::set_item(size_t idx, std::optional<id> o_item_id) -> void {
		_item_widgets[idx].o_item_id = o_item_id;
	}

	auto hotbar::set_on_click(std::function<void(std::optional<id>)> handler) -> void {
		for (auto& item_widget : _item_widgets) {
			item_widget.on_click = handler;
		}
	}

	auto hotbar::click(size_t idx) -> void {
		_most_recent_idx = idx;
		_item_widgets[idx].on_click(_item_widgets[idx].o_item_id);
	}
}
