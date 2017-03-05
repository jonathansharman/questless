/**
* @file    MessageDialog.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the MessageDialog class.
*/

#include "ui/MessageDialog.h"

using namespace sdl;
using namespace units;

namespace questless
{
	Dialog::State MessageDialog::update()
	{
		if(input().any_presses()) {
			return _cont();
		}
		return State::open;
	}

	void MessageDialog::draw() const
	{
		draw_title(_title);
		draw_prompt(_prompt);
	}

	void MessageDialog::load_textures()
	{
		_txt_title = make_title(_title);
		_txt_prompt = make_prompt(_prompt);
	}
}
