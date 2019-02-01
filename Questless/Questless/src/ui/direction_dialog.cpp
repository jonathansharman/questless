//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "ui/direction_dialog.hpp"

using namespace media;

namespace ql {
	dialog::state direction_dialog::update(input_manager& im) {
		if (im.pressed({sf::Keyboard::Backspace, sf::Keyboard::Escape})) { return _cont(std::nullopt); }

		if (im.pressed(sf::Keyboard::E)) {
			return _cont(region_tile::direction::one);
		} else if (im.pressed(sf::Keyboard::W)) {
			return _cont(region_tile::direction::two);
		} else if (im.pressed(sf::Keyboard::Q)) {
			return _cont(region_tile::direction::three);
		} else if (im.pressed(sf::Keyboard::A)) {
			return _cont(region_tile::direction::four);
		} else if (im.pressed(sf::Keyboard::S)) {
			return _cont(region_tile::direction::five);
		} else if (im.pressed(sf::Keyboard::D)) {
			return _cont(region_tile::direction::six);
		}

		return state::open;
	}

	void direction_dialog::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		target.draw(_title, states);
		target.draw(_prompt, states);
	}
}
