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

namespace questless
{
	void CountDialog::update(const Input& input)
	{
		if (input.presses(SDLK_BACKSPACE) || input.presses(SDLK_ESCAPE)) {
			close();
			return _cont(boost::none);
		}

		_count += input.presses(SDLK_UP);
		_count -= input.presses(SDLK_UP);
		_count = (_min && _count < _min.value()) ? _min.value() : _count;
		_count = (_max && _count > _max.value()) ? _max.value() : _count;

		if (input.pressed(MouseButton::left) || input.presses(SDLK_RETURN)) {
			if (_predicate(_count)) {
				close();
				return _cont(_count);
			}
		}
	}

	void CountDialog::draw(const Window& window)
	{
		int x_center = window.center().x;

		// Draw the title and prompt.
		renderer().draw_rect(Rect{x_center - _txt_title->width() / 2, 0, _txt_title->width(), _txt_title->height()}, Color{0, 0, 0, 128}, true);
		_txt_title->draw(Point{x_center, 0}, HAlign::center);
		renderer().draw_rect(Rect{x_center - _txt_prompt->width() / 2, _prompt_top, _txt_prompt->width(), _txt_prompt->height()}, Color{0, 0, 0, 128}, true);
		_txt_prompt->draw(Point{x_center, _prompt_top}, HAlign::center);

		// Draw the magnitude selector.
		Texture txt_current = font_manager()[io_font_handle()].render(std::to_string(static_cast<int>(_count)), renderer(), Color::white());
		renderer().draw_rect(Rect{x_center - txt_current.width() / 2, 100, txt_current.width(), txt_current.height()}, Color{0, 0, 0, 128}, true);
		txt_current.draw(Point{x_center, 100}, HAlign::center);
	}

	void CountDialog::refresh()
	{
		_txt_title = Texture::make(font_manager()[title_font_handle()].render(_title, renderer(), Color::white()));
		_txt_prompt = Texture::make(font_manager()[prompt_font_handle()].render(_prompt, renderer(), Color::white()));
	}
}
