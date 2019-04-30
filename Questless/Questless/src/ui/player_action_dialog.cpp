//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "player_action_dialog.hpp"
#include "hud.hpp"

using namespace media;

namespace ql {
	std::size_t player_action_dialog::_last_used = 0;

	void player_action_dialog::update(sec elapsed_time, input_manager& im) {
		if (!_hud.inventory_open()) {
			if (im.down(sf::Keyboard::X) || im.pressed({sf::Keyboard::Z, sf::Keyboard::Return})) {
				_cont(idle{im.shift()});
				close();
				return;
			} else if (im.pressed(sf::Keyboard::E)) {
				_cont(move{im.shift(), region_tile::direction::one});
				close();
				return;
			} else if (im.pressed(sf::Keyboard::W)) {
				_cont(move{im.shift(), region_tile::direction::two});
				close();
				return;
			} else if (im.pressed(sf::Keyboard::Q)) {
				_cont(move{im.shift(), region_tile::direction::three});
				close();
				return;
			} else if (im.pressed(sf::Keyboard::A)) {
				_cont(move{im.shift(), region_tile::direction::four});
				close();
				return;
			} else if (im.pressed(sf::Keyboard::S)) {
				_cont(move{im.shift(), region_tile::direction::five});
				close();
				return;
			} else if (im.pressed(sf::Keyboard::D)) {
				_cont(move{im.shift(), region_tile::direction::six});
				close();
				return;
			} else if (im.pressed(sf::Keyboard::Space)) {
				_cont(use{_last_used});
				close();
				return;
			} else if (im.pressed(sf::Keyboard::Num1)) {
				_last_used = 0;
				_cont(use{0});
				close();
				return;
			} else if (im.pressed(sf::Keyboard::Num2)) {
				_last_used = 1;
				_cont(use{1});
				close();
			} else if (im.pressed(sf::Keyboard::Num3)) {
				_last_used = 2;
				_cont(use{2});
				close();
			} else if (im.pressed(sf::Keyboard::Num4)) {
				_last_used = 3;
				_cont(use{3});
				close();
			} else if (im.pressed(sf::Keyboard::Num5)) {
				_last_used = 4;
				_cont(use{4});
				close();
			} else if (im.pressed(sf::Keyboard::Num6)) {
				_last_used = 5;
				_cont(use{5});
				close();
			} else if (im.pressed(sf::Keyboard::Num7)) {
				_last_used = 6;
				_cont(use{6});
				close();
			} else if (im.pressed(sf::Keyboard::Num8)) {
				_last_used = 7;
				_cont(use{7});
				close();
			} else if (im.pressed(sf::Keyboard::Num9)) {
				_last_used = 8;
				_cont(use{8});
				close();
			} else if (im.pressed(sf::Keyboard::Num0)) {
				_last_used = 9;
				_cont(use{9});
				close();
			}
		}
	}
}
