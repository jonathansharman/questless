//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "ui/Dialog.h"

#include "sdl/Font.h"
#include "sdl/resources.h"

#include "units/ScreenSpace.h"

using namespace sdl;
using namespace units;

namespace questless
{
	uptr<sdl::Texture> Dialog::make_title(char const* title, colors::Color color)
	{
		static auto title_font_handle = font_manager().add("resources/fonts/dumbledor1.ttf", 32);
		return std::make_unique<Texture>(font_manager()[title_font_handle].render(title, color));
	}
	uptr<sdl::Texture> Dialog::make_prompt(char const* prompt, colors::Color color)
	{
		static auto prompt_font_handle = font_manager().add("resources/fonts/dumbledor1.ttf", 20);
		return std::make_unique<Texture>(font_manager()[prompt_font_handle].render(prompt, color));
	}
	uptr<sdl::Texture> Dialog::make_selector(char const* selector, colors::Color color)
	{
		static auto selector_font_handle = font_manager().add("resources/fonts/dumbledor1.ttf", 32);
		return std::make_unique<Texture>(font_manager()[selector_font_handle].render(selector, color));
	}

	void Dialog::draw_title(Texture const& title)
	{
		int x_center = window().x_center();
		renderer().draw_box
			( ScreenSpace::Box
				{ ScreenSpace::Point{x_center - title.width() / 2, 0}
				, ScreenSpace::Vector{title.width(), title.height()}
				}
			, colors::black(0.5f)
			);
		title.draw(ScreenSpace::Point{x_center, 0}, TextureSpace::align_center);
	}
	void Dialog::draw_prompt(Texture const& prompt)
	{
		int x_center = window().x_center();
		renderer().draw_box
			( ScreenSpace::Box
				{ ScreenSpace::Point{x_center - prompt.width() / 2, _prompt_top}
				, ScreenSpace::Vector{prompt.width(), prompt.height()}
				}
			, colors::black(0.5f)
			);
		prompt.draw(ScreenSpace::Point{x_center, _prompt_top}, TextureSpace::align_center);
	}
	void Dialog::draw_selector(Texture const& selector)
	{
		int x_center = window().x_center();
		renderer().draw_box
			( ScreenSpace::Box
				{ ScreenSpace::Point{x_center - selector.width() / 2, _selector_top}
				, ScreenSpace::Vector{selector.width(), selector.height()}
				}
			, colors::black(0.5f)
			);
		selector.draw(ScreenSpace::Point{x_center, _selector_top}, TextureSpace::align_center);
	}
}
