//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "input_manager.hpp"
#include "quantities/wall_time.hpp"
#include "view_space.hpp"

namespace ql {
	//! A UI element.
	struct widget : public sf::Drawable {
		widget* parent;

		widget(widget* parent) : parent{parent} {}

		virtual ~widget() = default;

		//! The offset of this widget's position relative to its parent.
		virtual view::vector get_local_offset() const = 0;

		//! The position of this widget relative to the window.
		view::point get_position() const {
			return (parent ? parent->get_position() : view::point{}) + get_local_offset();
		}

		//! The size of this widget's bounding box.
		virtual view::vector get_size() const = 0;

		//! A bounding box relative to the window, containing this widget.
		view::box get_bounding_box() const {
			return {get_position(), get_size()};
		}

		//! Updates the widget. To be called once per frame.
		//! @param elapsed_time The amount of time since the last update.
		//! @param im The input manager.
		virtual void update(sec elapsed_time, std::vector<sf::Event>& events) = 0;
	};
}
