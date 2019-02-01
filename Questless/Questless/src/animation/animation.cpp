//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "animation.hpp"

namespace ql {
	void animation::update(sec elapsed_time) {
		if (!_stopped && !_paused) {
			animation_subupdate(elapsed_time * time_scale.value());
		}
	}

	void animation::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		states.transform *= getTransform();
		animation_subdraw(target, states);
	}
}
