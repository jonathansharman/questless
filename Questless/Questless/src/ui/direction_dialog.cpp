//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "ui/direction_dialog.hpp"

using namespace media;

namespace ql {
	void direction_dialog::update(sec elapsed_time, input_manager& im) {
		if (im.pressed({sf::Keyboard::Backspace, sf::Keyboard::Escape})) {
			_cont(std::nullopt);
			close();
			return;
		}

		if (im.pressed(sf::Keyboard::E)) {
			_cont(region_tile::direction::one);
			close();
			return;
		} else if (im.pressed(sf::Keyboard::W)) {
			_cont(region_tile::direction::two);
			close();
			return;
		} else if (im.pressed(sf::Keyboard::Q)) {
			_cont(region_tile::direction::three);
			close();
			return;
		} else if (im.pressed(sf::Keyboard::A)) {
			_cont(region_tile::direction::four);
			close();
			return;
		} else if (im.pressed(sf::Keyboard::S)) {
			_cont(region_tile::direction::five);
			close();
			return;
		} else if (im.pressed(sf::Keyboard::D)) {
			_cont(region_tile::direction::six);
			close();
			return;
		}
	}

	void direction_dialog::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		target.draw(_title, states);
		target.draw(_prompt, states);
	}
}
