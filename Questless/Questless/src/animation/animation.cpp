//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#include "animation.hpp"

namespace ql {
	auto animation::update(sec elapsed_time) -> void {
		if (!_stopped && !_paused) { animation_subupdate(elapsed_time * time_scale.get()); }
	}

	auto animation::draw(sf::RenderTarget& target, sf::RenderStates states) const -> void {
		states.transform *= getTransform();
		animation_subdraw(target, states);
	}
}
