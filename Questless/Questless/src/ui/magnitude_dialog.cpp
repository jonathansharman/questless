//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "ui/magnitude_dialog.hpp"

#include "sdl/resources.hpp"

using namespace sdl;
using namespace units;

namespace ql
{
	dialog::state magnitude_dialog::update()
	{
		if (the_input().presses(SDLK_BACKSPACE) || the_input().presses(SDLK_ESCAPE)) {
			return _cont(std::nullopt);
		}

		double old_magnitude = _magnitude;

		_magnitude += (the_input().shift() ? 10.0 : 1.0) * (the_input().presses(SDLK_UP) - the_input().presses(SDLK_DOWN) + the_input().scroll());
		_magnitude = (_min && _magnitude < _min.value()) ? _min.value() : _magnitude;
		_magnitude = (_max && _magnitude > _max.value()) ? _max.value() : _magnitude;

		// Update selector texture if value changed.
		if (old_magnitude != _magnitude) {
			_txt_selector = make_selector(std::to_string(_magnitude).c_str());
		}

		if (the_input().pressed(mouse_button::left) || the_input().presses(SDLK_RETURN) || the_input().presses(SDLK_SPACE)) {
			return _cont(_magnitude);
		}
		return state::open;
	}

	void magnitude_dialog::draw() const
	{
		draw_title(*_txt_title);
		draw_prompt(*_txt_prompt);
		draw_selector(*_txt_selector);
	}

	void magnitude_dialog::load_textures()
	{
		_txt_title = make_title(_title.c_str());
		_txt_prompt = make_prompt(_prompt.c_str());
		_txt_selector = make_selector(std::to_string(_magnitude).c_str());
	}
}
