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
	void PlayerActionDialog::update(const Input& input)
	{
		if (input.presses(SDLK_z) || input.presses(SDLK_RETURN)) {
			close();
			return _cont({Choice::Type::idle, 0});
		} else if (input.presses(SDLK_d)) {
			close();
			return _cont({Choice::Type::move, 1});
		} else if (input.presses(SDLK_e)) {
			close();
			return _cont({Choice::Type::move, 2});
		} else if (input.presses(SDLK_w)) {
			close();
			return _cont({Choice::Type::move, 3});
		} else if (input.presses(SDLK_q)) {
			close();
			return _cont({Choice::Type::move, 4});
		} else if (input.presses(SDLK_a)) {
			close();
			return _cont({Choice::Type::move, 5});
		} else if (input.presses(SDLK_s)) {
			close();
			return _cont({Choice::Type::move, 6});
		} else if (input.presses(SDLK_1)) {
			close();
			return _cont({Choice::Type::use, 0});
		} else if (input.presses(SDLK_2)) {
			close();
			return _cont({Choice::Type::use, 1});
		} else if (input.presses(SDLK_3)) {
			close();
			return _cont({Choice::Type::use, 2});
		} else if (input.presses(SDLK_4)) {
			close();
			return _cont({Choice::Type::use, 3});
		} else if (input.presses(SDLK_5)) {
			close();
			return _cont({Choice::Type::use, 4});
		} else if (input.presses(SDLK_6)) {
			close();
			return _cont({Choice::Type::use, 5});
		} else if (input.presses(SDLK_7)) {
			close();
			return _cont({Choice::Type::use, 6});
		} else if (input.presses(SDLK_8)) {
			close();
			return _cont({Choice::Type::use, 7});
		} else if (input.presses(SDLK_9)) {
			close();
			return _cont({Choice::Type::use, 8});
		} else if (input.presses(SDLK_0)) {
			close();
			return _cont({Choice::Type::use, 9});
		}
	}
}
