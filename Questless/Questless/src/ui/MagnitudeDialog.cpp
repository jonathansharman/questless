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
using namespace units;

namespace questless
{
	bool MagnitudeDialog::update(Input& input)
	{
		if (input.presses(SDLK_BACKSPACE) || input.presses(SDLK_ESCAPE)) {
			return _cont(boost::none);
		}

		double previous_magnitude = _magnitude;
		_magnitude += 10.0 * (input.presses(SDLK_UP) - input.presses(SDLK_DOWN) + input.scroll());
		if (!_predicate(_magnitude)) {
			_magnitude = previous_magnitude;
		}
		_magnitude = (_min && _magnitude < _min.value()) ? _min.value() : _magnitude;
		_magnitude = (_max && _magnitude > _max.value()) ? _max.value() : _magnitude;

		if (input.pressed(MouseButton::left) || input.presses(SDLK_RETURN) || input.presses(SDLK_SPACE)) {
			return _cont(_magnitude);
		}
		return false;
	}

	void MagnitudeDialog::draw(const Window& window)
	{
		int x_center = window.center().x;

		// Draw the title and prompt.
		renderer().draw_rect(ScreenRect{x_center - _txt_title->width() / 2, 0, _txt_title->width(), _txt_title->height()}, Color::black(128), true);
		_txt_title->draw(ScreenPoint{x_center, 0}, HAlign::center);
		renderer().draw_rect(ScreenRect{x_center - _txt_prompt->width() / 2, _prompt_top, _txt_prompt->width(), _txt_prompt->height()}, Color::black(128), true);
		_txt_prompt->draw(ScreenPoint{x_center, _prompt_top}, HAlign::center);

		// Draw the magnitude selector.
		Texture txt_magnitude = font_manager()[io_font_handle()].render(std::to_string(static_cast<int>(_magnitude)), renderer(), Color::white());
		renderer().draw_rect(ScreenRect{x_center - txt_magnitude.width() / 2, 100, txt_magnitude.width(), txt_magnitude.height()}, Color::black(128), true);
		txt_magnitude.draw(ScreenPoint{x_center, 100}, HAlign::center);
	}

	void MagnitudeDialog::load_textures()
	{
		_txt_title = std::make_unique<Texture>(font_manager()[title_font_handle()].render(_title, renderer(), Color::white()));
		_txt_prompt = std::make_unique<Texture>(font_manager()[prompt_font_handle()].render(_prompt, renderer(), Color::white()));
	}
}
