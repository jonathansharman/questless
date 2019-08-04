//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "split_panel.hpp"

#include "utility/visitation.hpp"

namespace ql {
	using namespace view::literals;

	split_panel::split_panel(ql::split_line split_line) : split_line{split_line} {}

	auto split_panel::get_size() const -> view::vector {
		return _size;
	};

	auto split_panel::update(sec elapsed_time) -> void {
		if (_first) { _first->update(elapsed_time); }
		if (_second) { _second->update(elapsed_time); }
	}

	auto split_panel::draw(sf::RenderTarget& target, sf::RenderStates states) const -> void {
		if (_first) { target.draw(*_first, states); }
		if (_second) { target.draw(*_second, states); }
	}

	auto split_panel::set_position(view::point position) -> void {
		_position = position;
		arrange_first();
		arrange_second();
	}

	auto split_panel::get_position() const -> view::point {
		return _position;
	}

	auto split_panel::on_parent_resize(view::vector parent_size) -> void {
		_size = parent_size;
		arrange_first();
		arrange_second();
	}

	auto split_panel::set_first_child(widget* first_child) -> void {
		_first = first_child;
		arrange_first();
	}

	auto split_panel::set_second_child(widget* second_child) -> void {
		_second = second_child;
		arrange_second();
	}

	auto split_panel::arrange_first() -> void {
		if (_first) {
			_first->on_parent_resize(_size);
			_first->set_position(get_position());
		}
	}

	auto split_panel::arrange_second() -> void {
		if (_second) {
			_second->on_parent_resize(_size);
			switch (split_line) {
				case split_line::horizontal:
					auto const y = match(
						split_location,
						[](view::px px) { return px; },
						[&](float proportion) { return get_size()[1] * proportion; });
					_second->set_position(get_position() + view::vector{0.0_px, y});
					break;
				case split_line::vertical:
					auto const x = match(
						split_location,
						[](view::px px) { return px; },
						[&](float proportion) { return get_size()[0] * proportion; });
					_second->set_position(get_position() + view::vector{x, 0.0_px});
					break;
			}
		}
	}
}
