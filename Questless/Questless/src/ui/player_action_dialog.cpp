//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "player_action_dialog.hpp"
#include "hud.hpp"

using namespace media;

namespace ql {
	std::size_t player_action_dialog::_last_used = 0;

	dialog::state player_action_dialog::update(input_manager& im) {
		if (!_hud.inventory_open()) {
			if (im.down(sf::Keyboard::X) || im.pressed({sf::Keyboard::Z, sf::Keyboard::Return})) {
				return _cont(idle{im.shift()});
			} else if (im.pressed(sf::Keyboard::E)) {
				return _cont(move{im.shift(), region_tile::direction::one});
			} else if (im.pressed(sf::Keyboard::W)) {
				return _cont(move{im.shift(), region_tile::direction::two});
			} else if (im.pressed(sf::Keyboard::Q)) {
				return _cont(move{im.shift(), region_tile::direction::three});
			} else if (im.pressed(sf::Keyboard::A)) {
				return _cont(move{im.shift(), region_tile::direction::four});
			} else if (im.pressed(sf::Keyboard::S)) {
				return _cont(move{im.shift(), region_tile::direction::five});
			} else if (im.pressed(sf::Keyboard::D)) {
				return _cont(move{im.shift(), region_tile::direction::six});
			} else if (im.pressed(sf::Keyboard::Space)) {
				return _cont(use{_last_used});
			} else if (im.pressed(sf::Keyboard::Num1)) {
				_last_used = 0;
				return _cont(use{0});
			} else if (im.pressed(sf::Keyboard::Num2)) {
				_last_used = 1;
				return _cont(use{1});
			} else if (im.pressed(sf::Keyboard::Num3)) {
				_last_used = 2;
				return _cont(use{2});
			} else if (im.pressed(sf::Keyboard::Num4)) {
				_last_used = 3;
				return _cont(use{3});
			} else if (im.pressed(sf::Keyboard::Num5)) {
				_last_used = 4;
				return _cont(use{4});
			} else if (im.pressed(sf::Keyboard::Num6)) {
				_last_used = 5;
				return _cont(use{5});
			} else if (im.pressed(sf::Keyboard::Num7)) {
				_last_used = 6;
				return _cont(use{6});
			} else if (im.pressed(sf::Keyboard::Num8)) {
				_last_used = 7;
				return _cont(use{7});
			} else if (im.pressed(sf::Keyboard::Num9)) {
				_last_used = 8;
				return _cont(use{8});
			} else if (im.pressed(sf::Keyboard::Num0)) {
				_last_used = 9;
				return _cont(use{9});
			}
		}
		return state::open;
	}
}
