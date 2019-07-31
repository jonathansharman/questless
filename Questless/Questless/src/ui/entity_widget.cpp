//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "entity_widget.hpp"

#include "animation/animation.hpp"

namespace ql {
	view::vector entity_widget::get_size() const {
		//! @todo This.
		return {};
	}

	void entity_widget::update(sec elapsed_time, std::vector<sf::Event>& /*events*/) {
		_ani->update(elapsed_time);
	}

	void entity_widget::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		_ani->draw(target, states);
	}
}
