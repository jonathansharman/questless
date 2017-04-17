//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "ui/PlayerActionDialog.h"

using namespace sdl;

namespace questless
{
	int PlayerActionDialog::_last_used = 0;

	Dialog::State PlayerActionDialog::update()
	{
		if (!_hud.inventory_open()) {
			if (input().presses(SDLK_z) || input().presses(SDLK_RETURN)) {
				return _cont({Choice::Type::idle, input().shift() ? 1 : 0});
			} else if (input().presses(SDLK_e)) {
				return _cont({input().shift() ? Choice::Type::shift_move : Choice::Type::move, 1});
			} else if (input().presses(SDLK_w)) {
				return _cont({input().shift() ? Choice::Type::shift_move : Choice::Type::move, 2});
			} else if (input().presses(SDLK_q)) {
				return _cont({input().shift() ? Choice::Type::shift_move : Choice::Type::move, 3});
			} else if (input().presses(SDLK_a)) {
				return _cont({input().shift() ? Choice::Type::shift_move : Choice::Type::move, 4});
			} else if (input().presses(SDLK_s)) {
				return _cont({input().shift() ? Choice::Type::shift_move : Choice::Type::move, 5});
			} else if (input().presses(SDLK_d)) {
				return _cont({input().shift() ? Choice::Type::shift_move : Choice::Type::move, 6});
			} else if (input().presses(SDLK_SPACE)) {
				return _cont({Choice::Type::use, _last_used});
			} else if (input().presses(SDLK_1)) {
				_last_used = 0;
				return _cont({Choice::Type::use, 0});
			} else if (input().presses(SDLK_2)) {
				_last_used = 1;
				return _cont({Choice::Type::use, 1});
			} else if (input().presses(SDLK_3)) {
				_last_used = 2;
				return _cont({Choice::Type::use, 2});
			} else if (input().presses(SDLK_4)) {
				_last_used = 3;
				return _cont({Choice::Type::use, 3});
			} else if (input().presses(SDLK_5)) {
				_last_used = 4;
				return _cont({Choice::Type::use, 4});
			} else if (input().presses(SDLK_6)) {
				_last_used = 5;
				return _cont({Choice::Type::use, 5});
			} else if (input().presses(SDLK_7)) {
				_last_used = 6;
				return _cont({Choice::Type::use, 6});
			} else if (input().presses(SDLK_8)) {
				_last_used = 7;
				return _cont({Choice::Type::use, 7});
			} else if (input().presses(SDLK_9)) {
				_last_used = 8;
				return _cont({Choice::Type::use, 8});
			} else if (input().presses(SDLK_0)) {
				_last_used = 9;
				return _cont({Choice::Type::use, 9});
			}
		}
		return State::open;
	}
}
