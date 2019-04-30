//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "input_manager.hpp"
#include "quantities/wall_time.hpp"

#include <SFML/Graphics/Drawable.hpp>

namespace ql {
	//! A UI element.
	struct widget : public sf::Drawable {
		virtual ~widget() = default;

		//! Updates the widget. To be called once per frame.
		//! @param elapsed_time The amount of time since the last update.
		//! @return The state of this dialog after this update: either open or closed.
		virtual void update(sec elapsed_time, input_manager& im) = 0;
	};
}
