//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "ui/dialog.h"

#include "sdl/font.h"
#include "sdl/resources.h"

#include "units/screen_space.h"

using namespace sdl;
using namespace units;

namespace ql
{
	uptr<sdl::texture> dialog::make_title(char const* title, colors::color color)
	{
		static auto title_font_handle = the_font_manager().add("resources/fonts/dumbledor1.ttf", 32);
		return std::make_unique<texture>(the_font_manager()[title_font_handle].render(title, color));
	}
	uptr<sdl::texture> dialog::make_prompt(char const* prompt, colors::color color)
	{
		static auto prompt_font_handle = the_font_manager().add("resources/fonts/dumbledor1.ttf", 20);
		return std::make_unique<texture>(the_font_manager()[prompt_font_handle].render(prompt, color));
	}
	uptr<sdl::texture> dialog::make_selector(char const* selector, colors::color color)
	{
		static auto selector_font_handle = the_font_manager().add("resources/fonts/dumbledor1.ttf", 32);
		return std::make_unique<texture>(the_font_manager()[selector_font_handle].render(selector, color));
	}

	void dialog::draw_title(texture const& title)
	{
		int x_center = the_window().x_center();
		the_renderer().draw_box
			( screen_space::box
				{ screen_space::point{x_center - title.width() / 2, 0}
				, screen_space::vector{title.width(), title.height()}
				}
			, colors::black(0.5f)
			);
		title.draw(screen_space::point{x_center, 0}, texture_space::align_center);
	}
	void dialog::draw_prompt(texture const& prompt)
	{
		int x_center = the_window().x_center();
		the_renderer().draw_box
			( screen_space::box
				{ screen_space::point{x_center - prompt.width() / 2, _prompt_top}
				, screen_space::vector{prompt.width(), prompt.height()}
				}
			, colors::black(0.5f)
			);
		prompt.draw(screen_space::point{x_center, _prompt_top}, texture_space::align_center);
	}
	void dialog::draw_selector(texture const& selector)
	{
		int x_center = the_window().x_center();
		the_renderer().draw_box
			( screen_space::box
				{ screen_space::point{x_center - selector.width() / 2, _selector_top}
				, screen_space::vector{selector.width(), selector.height()}
				}
			, colors::black(0.5f)
			);
		selector.draw(screen_space::point{x_center, _selector_top}, texture_space::align_center);
	}
}
