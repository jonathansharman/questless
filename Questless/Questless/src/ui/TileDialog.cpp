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

namespace questless
{
	void TileDialog::update(const Input& input)
	{
		if (input.presses(SDLK_BACKSPACE) || input.presses(SDLK_ESCAPE)) {
			close();
			return _cont(nullopt);
		}

		if (input.pressed(MouseButton::left)) {
			HexCoords value = _camera.tile_hovered();
			if (_predicate(value)) {
				close();
				return _cont(value);
			}
		}
	}

	void TileDialog::draw(const Window& window)
	{
		int x_center = window.center().x;

		// Draw the title and prompt.
		renderer().draw_rect(Rect{x_center - _txt_title->width() / 2, 0, _txt_title->width(), _txt_title->height()}, Color{0, 0, 0, 128}, true);
		_txt_title->draw(Point{x_center, 0}, HAlign::center);
		renderer().draw_rect(Rect{x_center - _txt_prompt->width() / 2, _prompt_top, _txt_prompt->width(), _txt_prompt->height()}, Color{0, 0, 0, 128}, true);
		_txt_prompt->draw(Point{x_center, _prompt_top}, HAlign::center);

		// Highlight valid tiles.
		/// @todo This.
	}

	void TileDialog::load_textures()
	{
		_txt_title = Texture::make(font_manager()["Dialog/title"].render(_title, renderer(), Color::white()));
		_txt_prompt = Texture::make(font_manager()["Dialog/prompt"].render(_prompt, renderer(), Color::white()));
	}
}
