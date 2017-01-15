/**
* @file    TileDialog.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the TileDialog class.
*/

#include "ui/TileDialog.h"

using namespace sdl;
using namespace units;

namespace questless
{
	bool TileDialog::update(Input& input)
	{
		if (input.presses(SDLK_BACKSPACE) || input.presses(SDLK_ESCAPE)) {
			return _cont(boost::none);
		}

		if (_origin) {
			if (input.presses(SDLK_e)) {
				return _cont(_origin->neighbor(RegionTileCoords::Direction::one));
			} else if (input.presses(SDLK_w)) {
				return _cont(_origin->neighbor(RegionTileCoords::Direction::two));
			} else if (input.presses(SDLK_q)) {
				return _cont(_origin->neighbor(RegionTileCoords::Direction::three));
			} else if (input.presses(SDLK_a)) {
				return _cont(_origin->neighbor(RegionTileCoords::Direction::four));
			} else if (input.presses(SDLK_s)) {
				return _cont(_origin->neighbor(RegionTileCoords::Direction::five));
			} else if (input.presses(SDLK_d)) {
				return _cont(_origin->neighbor(RegionTileCoords::Direction::six));
			}
		}

		if (input.pressed(MouseButton::left)) {
			RegionTileCoords value = _camera.tile_hovered();
			if (_predicate(value)) {
				return _cont(value);
			}
		}
		return false;
	}

	void TileDialog::draw(const Window& window)
	{
		int x_center = window.center().x;

		// Draw the title and prompt.
		renderer().draw_rect(ScreenRect{x_center - _txt_title->width() / 2, 0, _txt_title->width(), _txt_title->height()}, Color::black(128), true);
		_txt_title->draw(ScreenPoint{x_center, 0}, HAlign::center);
		renderer().draw_rect(ScreenRect{x_center - _txt_prompt->width() / 2, _prompt_top, _txt_prompt->width(), _txt_prompt->height()}, Color::black(128), true);
		_txt_prompt->draw(ScreenPoint{x_center, _prompt_top}, HAlign::center);

		// Highlight valid tiles.
		/// @todo This.
	}

	void TileDialog::load_textures()
	{
		_txt_title = std::make_unique<Texture>(font_manager()[title_font_handle()].render(_title, renderer(), Color::white()));
		_txt_prompt = std::make_unique<Texture>(font_manager()[prompt_font_handle()].render(_prompt, renderer(), Color::white()));
	}
}
