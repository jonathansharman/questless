//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "object_widget.hpp"

#include "animation/animation.hpp"

namespace ql {
	void object_widget::update(sec elapsed_time, input_manager& im) {
		_ani->update(elapsed_time);
	}

	void object_widget::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		_ani->draw(target, states);
	}
}
