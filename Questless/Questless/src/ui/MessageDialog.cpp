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

namespace questless
{
	bool MessageDialog::update(Input& input)
	{
		if(input.presses(SDLK_BACKSPACE) || input.presses(SDLK_ESCAPE) || input.pressed(MouseButton::left) || input.presses(SDLK_RETURN) || input.presses(SDLK_SPACE)) {
			return _cont();
		}
		return false;
	}

	void MessageDialog::draw(const Window& window)
	{
		int x_center = window.center().x;

		// Draw the title and prompt.
		renderer().draw_rect(ScreenRect{x_center - _txt_title->width() / 2, 0, _txt_title->width(), _txt_title->height()}, Color::black(128), true);
		_txt_title->draw(ScreenPoint{x_center, 0}, HAlign::center);
		renderer().draw_rect(ScreenRect{x_center - _txt_prompt->width() / 2, _prompt_top, _txt_prompt->width(), _txt_prompt->height()}, Color::black(128), true);
		_txt_prompt->draw(ScreenPoint{x_center, _prompt_top}, HAlign::center);
	}

	void MessageDialog::load_textures()
	{
		_txt_title = Texture::make(font_manager()[title_font_handle()].render(_title, renderer(), Color::white()));
		_txt_prompt = Texture::make(font_manager()[prompt_font_handle()].render(_prompt, renderer(), Color::white()));
	}
}
