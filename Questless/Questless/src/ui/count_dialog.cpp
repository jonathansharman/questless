//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "ui/count_dialog.hpp"

using namespace sdl;
using namespace units;

namespace ql
{
	dialog::state count_dialog::update()
	{
		if (the_input().presses(SDLK_BACKSPACE) || the_input().presses(SDLK_ESCAPE)) {
			return _cont(std::nullopt);
		}

		int old_count = _count;

		_count += (the_input().shift() ? 10 : 1) * (the_input().presses(SDLK_UP) - the_input().presses(SDLK_DOWN) + the_input().scroll());
		_count = (_min && _count < *_min) ? *_min : _count;
		_count = (_max && _count > *_max) ? *_max : _count;

		// Update selector texture if value changed.
		if (old_count != _count) {
			_txt_selector = make_selector(std::to_string(_count).c_str());
		}

		if (the_input().pressed(mouse_button::left) || the_input().presses(SDLK_RETURN) || the_input().presses(SDLK_SPACE)) {
			return _cont(_count);
		}
		return state::open;
	}

	void count_dialog::draw() const
	{
		draw_title(*_txt_title);
		draw_prompt(*_txt_prompt);
		draw_selector(*_txt_selector);
	}

	void count_dialog::load_textures()
	{
		_txt_title = make_title(_title.c_str());
		_txt_prompt = make_prompt(_prompt.c_str());
		_txt_selector = make_selector(std::to_string(_count).c_str());
	}
}
