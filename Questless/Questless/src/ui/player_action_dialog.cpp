//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "ui/player_action_dialog.h"

using namespace sdl;

namespace ql
{
	std::size_t player_action_dialog::_last_used = 0;

	dialog::state player_action_dialog::update()
	{
		if (!_hud.inventory_open()) {
			if (the_input().presses(SDLK_z) || the_input().down(SDLK_x) || the_input().presses(SDLK_RETURN)) {
				return _cont(idle{the_input().shift()});
			} else if (the_input().presses(SDLK_e)) {
				return _cont(move{the_input().shift(), region_tile::direction::one});
			} else if (the_input().presses(SDLK_w)) {
				return _cont(move{the_input().shift(), region_tile::direction::two});
			} else if (the_input().presses(SDLK_q)) {
				return _cont(move{the_input().shift(), region_tile::direction::three});
			} else if (the_input().presses(SDLK_a)) {
				return _cont(move{the_input().shift(), region_tile::direction::four});
			} else if (the_input().presses(SDLK_s)) {
				return _cont(move{the_input().shift(), region_tile::direction::five});
			} else if (the_input().presses(SDLK_d)) {
				return _cont(move{the_input().shift(), region_tile::direction::six});
			} else if (the_input().presses(SDLK_SPACE)) {
				return _cont(use{_last_used});
			} else if (the_input().presses(SDLK_1)) {
				_last_used = 0;
				return _cont(use{0});
			} else if (the_input().presses(SDLK_2)) {
				_last_used = 1;
				return _cont(use{1});
			} else if (the_input().presses(SDLK_3)) {
				_last_used = 2;
				return _cont(use{2});
			} else if (the_input().presses(SDLK_4)) {
				_last_used = 3;
				return _cont(use{3});
			} else if (the_input().presses(SDLK_5)) {
				_last_used = 4;
				return _cont(use{4});
			} else if (the_input().presses(SDLK_6)) {
				_last_used = 5;
				return _cont(use{5});
			} else if (the_input().presses(SDLK_7)) {
				_last_used = 6;
				return _cont(use{6});
			} else if (the_input().presses(SDLK_8)) {
				_last_used = 7;
				return _cont(use{7});
			} else if (the_input().presses(SDLK_9)) {
				_last_used = 8;
				return _cont(use{8});
			} else if (the_input().presses(SDLK_0)) {
				_last_used = 9;
				return _cont(use{9});
			}
		}
		return state::open;
	}
}
