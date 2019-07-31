//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "widget.hpp"

namespace ql {
	//! A container widget that centers a single child widget within the window.
	struct window_widget : widget {
		//! @param child The widget contained in this widget.
		//! @param initial_size The initial size of the window area.
		window_widget(widget& child, view::vector initial_size)
			: widget{nullptr}
			, _child{child}
			, _size{initial_size} //
		{
			center_child_in_window();
		}

		view::vector get_size() const final {
			return _size;
		}

		void update(sec elapsed_time, std::vector<sf::Event>& events) final {
			for (auto event : events) {
				if (event.type == sf::Event::Resized) {
					// Update size.
					_size = view::vector_from_size_event(event.size);
					// Recenter the child widget.
					center_child_in_window();
				}
			}
			// Update the child widget.
			_child.update(elapsed_time, events);
		}

		void draw(sf::RenderTarget& target, sf::RenderStates states) const final {
			// Draw the child widget.
			target.draw(_child, states);
		}

	private:
		view::vector _size;
		widget& _child;

		//! Centers the child widget by setting its @p local_offset vector.
		void center_child_in_window() {
			_child.local_offset = (get_size() - _child.get_size()) / 2.0f;
		}
	};
}
