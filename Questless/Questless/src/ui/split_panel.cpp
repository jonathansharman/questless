//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "split_panel.hpp"

#include "utility/visitation.hpp"

namespace ql {
	split_panel::split_panel(widget& parent, ql::split_line split_line) : widget{&parent}, split_line{split_line} {}

	view::vector split_panel::get_size() const {
		return parent()->get_size();
	};

	void split_panel::update(sec elapsed_time, std::vector<sf::Event>& events) {
		for (auto event : events) {
			if (event.type == sf::Event::Resized) {
				if (_first) { arrange_first(); }
				if (_second) { arrange_second(); }
			}
		}
		if (_first) { _first->update(elapsed_time, events); }
		if (_second) { _second->update(elapsed_time, events); }
	}

	void split_panel::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		if (_first) { target.draw(*_first, states); }
		if (_second) { target.draw(*_second, states); }
	}

	void split_panel::set_first_child(widget* first_child) {
		_first = first_child;
		arrange_first();
	}

	void split_panel::set_second_child(widget* second_child) {
		_second = second_child;
		arrange_second();
	}

	void split_panel::arrange_first() {
		_first->local_offset = local_offset;
	}

	void split_panel::arrange_second() {
		// Initially set second child's local offset to the panel's.
		_second->local_offset = local_offset;
		// Then adjust according to the split orientation and location.
		switch (split_line) {
			case split_line::horizontal:
				auto const offset = match( //
					split_location,
					[](view::px px) { return px; },
					[&](float proportion) { return get_size()[1] * proportion; });
				_second->local_offset[1] += offset;
				break;
			case split_line::vertical:
				auto const offset = match( //
					split_location,
					[](view::px px) { return px; },
					[&](float proportion) { return get_size()[0] * proportion; });
				_second->local_offset[0] += offset;
				break;
		}
	}
}
