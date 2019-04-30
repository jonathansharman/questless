//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "widget.hpp"

#include "utility/reference.hpp"

namespace ql {
	struct animation;

	//! Allows interaction with an object in the world.
	struct object_widget : widget {
		void update(sec elapsed_time, input_manager& im) final;

		void draw(sf::RenderTarget& target, sf::RenderStates states) const final;

	private:
		uptr<animation> _ani;
	};
}
