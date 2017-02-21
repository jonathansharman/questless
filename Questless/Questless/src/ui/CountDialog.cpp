/**
* @file    MessageDialog.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the MessageDialog class.
*/

#include "ui/CountDialog.h"

using namespace sdl;
using namespace units;

namespace questless
{
	bool CountDialog::update(Input& input)
	{
		if (input.presses(SDLK_BACKSPACE) || input.presses(SDLK_ESCAPE)) {
			return _cont(boost::none);
		}

		_count += (input.shift() ? 10 : 1) * (input.presses(SDLK_UP) - input.presses(SDLK_DOWN) + input.scroll());
		_count = (_min && _count < *_min) ? *_min : _count;
		_count = (_max && _count > *_max) ? *_max : _count;

		if (input.pressed(MouseButton::left) || input.presses(SDLK_RETURN) || input.presses(SDLK_SPACE)) {
			return _cont(_count);
		}
		return false;
	}

	void CountDialog::draw(Window const& window)
	{
		int x_center = window.center().x;

		// Draw the title and prompt.
		renderer().draw_rect(ScreenRect{x_center - _txt_title->width() / 2, 0, _txt_title->width(), _txt_title->height()}, Color::black(128), true);
		_txt_title->draw(ScreenPoint{x_center, 0}, HAlign::center);
		renderer().draw_rect(ScreenRect{x_center - _txt_prompt->width() / 2, _prompt_top, _txt_prompt->width(), _txt_prompt->height()}, Color::black(128), true);
		_txt_prompt->draw(ScreenPoint{x_center, _prompt_top}, HAlign::center);

		// Draw the count selector.
		Texture txt_count = font_manager()[io_font_handle()].render(std::to_string(static_cast<int>(_count)), renderer(), Color::white());
		renderer().draw_rect(ScreenRect{x_center - txt_count.width() / 2, 100, txt_count.width(), txt_count.height()}, Color::black(128), true);
		txt_count.draw(ScreenPoint{x_center, 100}, HAlign::center);
	}

	void CountDialog::refresh()
	{
		_txt_title = std::make_unique<Texture>(font_manager()[title_font_handle()].render(_title, renderer(), Color::white()));
		_txt_prompt = std::make_unique<Texture>(font_manager()[prompt_font_handle()].render(_prompt, renderer(), Color::white()));
	}
}
