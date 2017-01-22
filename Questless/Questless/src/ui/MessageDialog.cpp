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
	bool MessageDialog::update(Input& input)
	{
		if(input.any_presses()) {
			return _cont();
		}
		return false;
	}

	void MessageDialog::draw(Window const& window)
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
		_txt_title = std::make_unique<Texture>(font_manager()[title_font_handle()].render(_title, renderer(), Color::white()));
		_txt_prompt = std::make_unique<Texture>(font_manager()[prompt_font_handle()].render(_prompt, renderer(), Color::white()));
	}
}
