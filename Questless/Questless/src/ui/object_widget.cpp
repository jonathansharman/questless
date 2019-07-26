//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "being_widget.hpp"

#include "animation/animation.hpp"

namespace ql {
	void being_widget::update(sec elapsed_time, std::vector<sf::Event>& events) {
		_ani->update(elapsed_time);
	}

	void being_widget::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		_ani->draw(target, states);
	}
}
