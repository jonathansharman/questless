//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "ui/tile_dialog.hpp"

#include "game.hpp"

using namespace sdl;
using namespace units;

namespace ql {
	dialog::state tile_dialog::update() {
		if (the_input().presses(SDLK_BACKSPACE) || the_input().presses(SDLK_ESCAPE)) {
			the_game().world_renderer().clear_highlight_predicate();
			return _cont(std::nullopt);
		}

		std::optional<region_tile::point> result;

		if (_origin) {
			if (the_input().presses(SDLK_e)) {
				result = _origin->neighbor(region_tile::direction::one);
			} else if (the_input().presses(SDLK_w)) {
				result = _origin->neighbor(region_tile::direction::two);
			} else if (the_input().presses(SDLK_q)) {
				result = _origin->neighbor(region_tile::direction::three);
			} else if (the_input().presses(SDLK_a)) {
				result = _origin->neighbor(region_tile::direction::four);
			} else if (the_input().presses(SDLK_s)) {
				result = _origin->neighbor(region_tile::direction::five);
			} else if (the_input().presses(SDLK_d)) {
				result = _origin->neighbor(region_tile::direction::six);
			}
			if (result && _predicate(*result)) {
				the_game().world_renderer().clear_highlight_predicate();
				return _cont(*result);
			}
		}

		if (the_input().pressed(mouse_button::left)) {
			result = the_game().camera().tile_hovered();
			if (_predicate(*result)) {
				the_game().world_renderer().clear_highlight_predicate();
				return _cont(*result);
			}
		}

		the_game().world_renderer().set_highlight_predicate(_predicate);
		return state::open;
	}

	void tile_dialog::draw() const {
		draw_title(*_txt_title);
		draw_prompt(*_txt_prompt);

		// Highlight valid tiles.
		//! @todo This.
	}

	void tile_dialog::load_textures() {
		_txt_title = make_title(_title.c_str());
		_txt_prompt = make_prompt(_prompt.c_str());
	}
}
