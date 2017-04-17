//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

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
		draw_title(*_txt_title);
		draw_prompt(*_txt_prompt);
	}

	void MessageDialog::load_textures()
	{
		_txt_title = make_title(_title.c_str());
		_txt_prompt = make_prompt(_prompt.c_str());
	}
}
