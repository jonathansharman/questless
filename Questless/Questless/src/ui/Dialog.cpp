/**
* @file    Dialog.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the Dialog abstract base class.
*/

#include "ui/Dialog.h"

#include "sdl/Font.h"
#include "sdl/resources.h"

#include "units/ScreenPoint.h"
#include "units/ScreenRect.h"

using namespace units;
using namespace sdl;

namespace questless
{
	sdl::Texture::ptr Dialog::make_title(std::string const& title)
	{
		static auto title_font_handle = font_manager().add("resources/fonts/dumbledor1.ttf", 32);
		return std::make_unique<Texture>(font_manager()[title_font_handle].render(title, Color::white()));
	}
	sdl::Texture::ptr Dialog::make_prompt(std::string const& prompt)
	{
		static auto prompt_font_handle = font_manager().add("resources/fonts/dumbledor1.ttf", 20);
		return std::make_unique<Texture>(font_manager()[prompt_font_handle].render(prompt, Color::white()));
	}
	sdl::Texture::ptr Dialog::make_selector(std::string const& selector)
	{
		static auto selector_font_handle = font_manager().add("resources/fonts/dumbledor1.ttf", 32);
		return std::make_unique<Texture>(font_manager()[selector_font_handle].render(selector, Color::white()));
	}

	void Dialog::draw_title(Texture const& title)
	{
		int x_center = window().center().x;
		renderer().draw_rect(ScreenRect{x_center - title.width() / 2, 0, title.width(), title.height()}, Color::black(128), true);
		title.draw(ScreenPoint{x_center, 0}, HAlign::center);
	}
	void Dialog::draw_prompt(Texture const& prompt)
	{
		int x_center = window().center().x;
		renderer().draw_rect(ScreenRect{x_center - prompt.width() / 2, _prompt_top, prompt.width(), prompt.height()}, Color::black(128), true);
		prompt.draw(ScreenPoint{x_center, _prompt_top}, HAlign::center);
	}
	void Dialog::draw_selector(Texture const& selector)
	{
		int x_center = window().center().x;
		renderer().draw_rect(ScreenRect{x_center - selector.width() / 2, _selector_top, selector.width(), selector.height()}, Color::black(128), true);
		selector.draw(ScreenPoint{x_center, _selector_top}, HAlign::center);
	}
}
