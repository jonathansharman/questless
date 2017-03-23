/**
* @file    MessageDialog.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the MessageDialog class.
*/

#include "ui/CountDialog.h"

using namespace sdl;
using namespace units;

namespace questless
{
	Dialog::State CountDialog::update()
	{
		if (input().presses(SDLK_BACKSPACE) || input().presses(SDLK_ESCAPE)) {
			return _cont(std::nullopt);
		}

		int old_count = _count;

		_count += (input().shift() ? 10 : 1) * (input().presses(SDLK_UP) - input().presses(SDLK_DOWN) + input().scroll());
		_count = (_min && _count < *_min) ? *_min : _count;
		_count = (_max && _count > *_max) ? *_max : _count;

		// Update selector texture if value changed.
		if (old_count != _count) {
			_txt_selector = make_selector(std::to_string(_count));
		}

		if (input().pressed(MouseButton::left) || input().presses(SDLK_RETURN) || input().presses(SDLK_SPACE)) {
			return _cont(_count);
		}
		return State::open;
	}

	void CountDialog::draw() const
	{
		draw_title(*_txt_title);
		draw_prompt(*_txt_prompt);
		draw_selector(*_txt_selector);
	}

	void CountDialog::load_textures()
	{
		_txt_title = make_title(_title);
		_txt_prompt = make_prompt(_prompt);
		_txt_selector = make_selector(std::to_string(_count));
	}
}
