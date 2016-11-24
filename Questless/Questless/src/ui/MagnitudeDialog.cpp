/**
* @file    MessageDialog.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the MagnitudeDialog class.
*/

#include "ui/MagnitudeDialog.h"

using namespace sdl;

namespace questless
{
	bool MagnitudeDialog::update(const Input& input)
	{
		if (input.presses(SDLK_BACKSPACE) || input.presses(SDLK_ESCAPE)) {
			return _cont(boost::none);
		}

		_magnitude += 10.0 * input.presses(SDLK_UP);
		_magnitude -= 10.0 * input.presses(SDLK_DOWN);
		_magnitude = (_min && _magnitude < _min.value()) ? _min.value() : _magnitude;
		_magnitude = (_max && _magnitude > _max.value()) ? _max.value() : _magnitude;

		if (input.pressed(MouseButton::left) || input.presses(SDLK_RETURN)) {
			if (_predicate(_magnitude)) {
				return _cont(_magnitude);
			}
		}
		return false;
	}

	void MagnitudeDialog::draw(const Window& window)
	{
		int x_center = window.center().x;

		// Draw the title and prompt.
		renderer().draw_rect(Rect{x_center - _txt_title->width() / 2, 0, _txt_title->width(), _txt_title->height()}, Color{0, 0, 0, 128}, true);
		_txt_title->draw(Point{x_center, 0}, HAlign::center);
		renderer().draw_rect(Rect{x_center - _txt_prompt->width() / 2, _prompt_top, _txt_prompt->width(), _txt_prompt->height()}, Color{0, 0, 0, 128}, true);
		_txt_prompt->draw(Point{x_center, _prompt_top}, HAlign::center);

		// Draw the magnitude selector.
		Texture txt_magnitude = font_manager()[io_font_handle()].render(std::to_string(static_cast<int>(_magnitude)), renderer(), Color::white());
		renderer().draw_rect(Rect{x_center - txt_magnitude.width() / 2, 100, txt_magnitude.width(), txt_magnitude.height()}, Color{0, 0, 0, 128}, true);
		txt_magnitude.draw(Point{x_center, 100}, HAlign::center);
	}

	void MagnitudeDialog::load_textures()
	{
		_txt_title = Texture::make(font_manager()[title_font_handle()].render(_title, renderer(), Color::white()));
		_txt_prompt = Texture::make(font_manager()[prompt_font_handle()].render(_prompt, renderer(), Color::white()));
	}
}
