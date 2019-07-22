//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "input_manager.hpp"
#include "quantities/wall_time.hpp"

#include <SFML/Graphics.hpp>

namespace ql {
	//! A UI element.
	struct widget : public sf::Drawable {
		widget* parent;

		widget(widget* parent) : parent{parent} {}

		virtual ~widget() = default;

		//! A bounding box relative to the window, containing this widget.
		virtual sf::FloatRect get_bounding_box() const = 0;

		//! Updates the widget. To be called once per frame.
		//! @param elapsed_time The amount of time since the last update.
		//! @param im The input manager.
		virtual void update(sec elapsed_time, input_manager& im) = 0;
	};
}
