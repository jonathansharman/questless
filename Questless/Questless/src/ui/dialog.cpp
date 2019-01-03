//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "ui/dialog.hpp"

#include "sdl/font.hpp"
#include "sdl/resources.hpp"

#include "units/spaces::window.hpp"

using namespace sdl;
using namespace units;

namespace ql {
	uptr<sdl::texture> dialog::make_title(char const* title, colors::color color) {
		static auto title_font_handle = the_font_manager().add("resources/fonts/dumbledor1.ttf", 32);
		return umake<texture>(the_font_manager()[title_font_handle].render(title, color));
	}
	uptr<sdl::texture> dialog::make_prompt(char const* prompt, colors::color color) {
		static auto prompt_font_handle = the_font_manager().add("resources/fonts/dumbledor1.ttf", 20);
		return umake<texture>(the_font_manager()[prompt_font_handle].render(prompt, color));
	}
	uptr<sdl::texture> dialog::make_selector(char const* selector, colors::color color) {
		static auto selector_font_handle = the_font_manager().add("resources/fonts/dumbledor1.ttf", 32);
		return umake<texture>(the_font_manager()[selector_font_handle].render(selector, color));
	}

	void dialog::draw_title(texture const& title) {
		int x_center = the_window().window_center().x();
		the_renderer().draw_box
			( spaces::window::box
				{ spaces::window::point{x_center - title.width() / 2, 0}
				, spaces::window::vector{title.width(), title.height()}
				}
			, colors::black(0.5f)
			);
		title.draw(spaces::window::point{x_center, 0}, spaces::window::align_center);
	}
	void dialog::draw_prompt(texture const& prompt) {
		int x_center = the_window().window_center().x();
		the_renderer().draw_box
			( spaces::window::box
				{ spaces::window::point{x_center - prompt.width() / 2, _prompt_top}
				, spaces::window::vector{prompt.width(), prompt.height()}
				}
			, colors::black(0.5f)
			);
		prompt.draw(spaces::window::point{x_center, _prompt_top}, spaces::window::align_center);
	}
	void dialog::draw_selector(texture const& selector) {
		int x_center = the_window().window_center().x();
		the_renderer().draw_box
			( spaces::window::box
				{ spaces::window::point{x_center - selector.width() / 2, _selector_top}
				, spaces::window::vector{selector.width(), selector.height()}
				}
			, colors::black(0.5f)
			);
		selector.draw(spaces::window::point{x_center, _selector_top}, spaces::window::align_center);
	}
}
