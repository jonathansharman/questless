//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "still_shape.hpp"

namespace ql {
	void still_shape::animation_subdraw(sf::RenderTarget& target, sf::RenderStates states) const {
		target.draw(*shape, states);
	}
}
