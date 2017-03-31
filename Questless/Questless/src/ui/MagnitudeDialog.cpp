/**
* @file    MessageDialog.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the MagnitudeDialog class.
*/

#include "ui/MagnitudeDialog.h"

using namespace sdl;
using namespace units;

namespace questless
{
	Dialog::State MagnitudeDialog::update()
	{
		if (input().presses(SDLK_BACKSPACE) || input().presses(SDLK_ESCAPE)) {
			return _cont(std::nullopt);
		}

		double old_magnitude = _magnitude;

		_magnitude += (input().shift() ? 10.0 : 1.0) * (input().presses(SDLK_UP) - input().presses(SDLK_DOWN) + input().scroll());
		_magnitude = (_min && _magnitude < _min.value()) ? _min.value() : _magnitude;
		_magnitude = (_max && _magnitude > _max.value()) ? _max.value() : _magnitude;

		// Update selector texture if value changed.
		if (old_magnitude != _magnitude) {
			_txt_selector = make_selector(std::to_string(_magnitude).c_str());
		}

		if (input().pressed(MouseButton::left) || input().presses(SDLK_RETURN) || input().presses(SDLK_SPACE)) {
			return _cont(_magnitude);
		}
		return State::open;
	}

	void MagnitudeDialog::draw() const
	{
		draw_title(*_txt_title);
		draw_prompt(*_txt_prompt);
		draw_selector(*_txt_selector);
	}

	void MagnitudeDialog::load_textures()
	{
		_txt_title = make_title(_title.c_str());
		_txt_prompt = make_prompt(_prompt.c_str());
		_txt_selector = make_selector(std::to_string(_magnitude).c_str());
	}
}
