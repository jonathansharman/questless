//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "ui/direction_dialog.h"

using namespace sdl;
using namespace units;

namespace ql
{
	dialog::state direction_dialog::update()
	{
		if (the_input().presses(SDLK_BACKSPACE) || the_input().presses(SDLK_ESCAPE)) {
			return _cont(std::nullopt);
		}

		if (the_input().presses(SDLK_e)) {
			return _cont(region_tile::direction::one);
		} else if (the_input().presses(SDLK_w)) {
			return _cont(region_tile::direction::two);
		} else if (the_input().presses(SDLK_q)) {
			return _cont(region_tile::direction::three);
		} else if (the_input().presses(SDLK_a)) {
			return _cont(region_tile::direction::four);
		} else if (the_input().presses(SDLK_s)) {
			return _cont(region_tile::direction::five);
		} else if (the_input().presses(SDLK_d)) {
			return _cont(region_tile::direction::six);
		}

		return state::open;
	}

	void direction_dialog::draw() const
	{
		draw_title(*_txt_title);
		draw_prompt(*_txt_prompt);
	}

	void direction_dialog::load_textures()
	{
		_txt_title = make_title(_title.c_str());
		_txt_prompt = make_prompt(_prompt.c_str());
	}
}
