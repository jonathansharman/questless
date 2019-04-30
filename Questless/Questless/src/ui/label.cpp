//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "label.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace ql {
	void label::update(sec, input_manager&) {}

	void label::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		target.draw(text, states);
	}
}
