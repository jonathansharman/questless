//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "ui/message_dialog.hpp"

#include "sdl/resources.hpp"

using namespace sdl;
using namespace units;

namespace ql
{
	dialog::state message_dialog::update()
	{
		if(the_input().any_presses()) {
			return _cont();
		}
		return state::open;
	}

	void message_dialog::draw() const
	{
		draw_title(*_txt_title);
		draw_prompt(*_txt_prompt);
	}

	void message_dialog::load_textures()
	{
		_txt_title = make_title(_title.c_str());
		_txt_prompt = make_prompt(_prompt.c_str());
	}
}
