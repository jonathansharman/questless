//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "ui/dialog/direction_dialog.hpp"

namespace ql {
	void direction_dialog::update(sec elapsed_time, std::vector<sf::Event>& events) {
		if (im.pressed({sf::Keyboard::Backspace, sf::Keyboard::Escape})) {
			_promise.set_value(std::nullopt);
			return;
		}

		if (im.pressed(sf::Keyboard::E)) {
			_promise.set_value(region_tile::direction::one);
			return;
		} else if (im.pressed(sf::Keyboard::W)) {
			_promise.set_value(region_tile::direction::two);
			return;
		} else if (im.pressed(sf::Keyboard::Q)) {
			_promise.set_value(region_tile::direction::three);
			return;
		} else if (im.pressed(sf::Keyboard::A)) {
			_promise.set_value(region_tile::direction::four);
			return;
		} else if (im.pressed(sf::Keyboard::S)) {
			_promise.set_value(region_tile::direction::five);
			return;
		} else if (im.pressed(sf::Keyboard::D)) {
			_promise.set_value(region_tile::direction::six);
			return;
		}
	}

	void direction_dialog::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		target.draw(_title, states);
		target.draw(_prompt, states);
	}
}
