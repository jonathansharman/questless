//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#include "still_shape.hpp"

namespace ql {
	auto still_shape::animation_subdraw(sf::RenderTarget& target, sf::RenderStates states) const -> void {
		target.draw(*shape, states);
	}
}
