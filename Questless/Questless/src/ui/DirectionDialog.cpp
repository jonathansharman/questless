/**
* @file    DirectionDialog.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*/

#include "ui/DirectionDialog.h"

using namespace sdl;
using namespace units;

namespace questless
{
	Dialog::State DirectionDialog::update()
	{
		if (input().presses(SDLK_BACKSPACE) || input().presses(SDLK_ESCAPE)) {
			return _cont(std::nullopt);
		}

		if (input().presses(SDLK_e)) {
			return _cont(RegionTileCoords::Direction::one);
		} else if (input().presses(SDLK_w)) {
			return _cont(RegionTileCoords::Direction::two);
		} else if (input().presses(SDLK_q)) {
			return _cont(RegionTileCoords::Direction::three);
		} else if (input().presses(SDLK_a)) {
			return _cont(RegionTileCoords::Direction::four);
		} else if (input().presses(SDLK_s)) {
			return _cont(RegionTileCoords::Direction::five);
		} else if (input().presses(SDLK_d)) {
			return _cont(RegionTileCoords::Direction::six);
		}

		return State::open;
	}

	void DirectionDialog::draw() const
	{
		draw_title(*_txt_title);
		draw_prompt(*_txt_prompt);
	}

	void DirectionDialog::load_textures()
	{
		_txt_title = make_title(_title.c_str());
		_txt_prompt = make_prompt(_prompt.c_str());
	}
}
