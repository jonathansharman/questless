/**
* @file    PlayerActionDialog.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the PlayerActionDialog class.
*/

#include "ui/PlayerActionDialog.h"

namespace questless
{
	bool PlayerActionDialog::update(sdl::Input& input)
	{
		if (input.presses(SDLK_z) || input.presses(SDLK_RETURN)) {
			return _cont({Choice::Type::idle, input.shift() ? 1 : 0});
		} else if (input.presses(SDLK_e)) {
			return _cont({Choice::Type::walk, 1});
		} else if (input.presses(SDLK_w)) {
			return _cont({Choice::Type::walk, 2});
		} else if (input.presses(SDLK_q)) {
			return _cont({Choice::Type::walk, 3});
		} else if (input.presses(SDLK_a)) {
			return _cont({Choice::Type::walk, 4});
		} else if (input.presses(SDLK_s)) {
			return _cont({Choice::Type::walk, 5});
		} else if (input.presses(SDLK_d)) {
			return _cont({Choice::Type::walk, 6});
		} else if (input.presses(SDLK_1)) {
			return _cont({Choice::Type::use, 0});
		} else if (input.presses(SDLK_2)) {
			return _cont({Choice::Type::use, 1});
		} else if (input.presses(SDLK_3)) {
			return _cont({Choice::Type::use, 2});
		} else if (input.presses(SDLK_4)) {
			return _cont({Choice::Type::use, 3});
		} else if (input.presses(SDLK_5)) {
			return _cont({Choice::Type::use, 4});
		} else if (input.presses(SDLK_6)) {
			return _cont({Choice::Type::use, 5});
		} else if (input.presses(SDLK_7)) {
			return _cont({Choice::Type::use, 6});
		} else if (input.presses(SDLK_8)) {
			return _cont({Choice::Type::use, 7});
		} else if (input.presses(SDLK_9)) {
			return _cont({Choice::Type::use, 8});
		} else if (input.presses(SDLK_0)) {
			return _cont({Choice::Type::use, 9});
		}
		return false;
	}
}
