//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "ui/vector_dialog.hpp"

#include "game.hpp"

using namespace sdl;
using namespace units;

namespace ql {
	dialog::state vector_dialog::update() {
		if (the_input().presses(SDLK_BACKSPACE) || the_input().presses(SDLK_ESCAPE)) {
			return _cont(std::nullopt);
		}

		std::optional<region_tile::vector> result;

		if (the_input().presses(SDLK_e)) {
			result = region_tile::vector::unit(region_tile::direction::one);
		} else if (the_input().presses(SDLK_w)) {
			result = region_tile::vector::unit(region_tile::direction::two);
		} else if (the_input().presses(SDLK_q)) {
			result = region_tile::vector::unit(region_tile::direction::three);
		} else if (the_input().presses(SDLK_a)) {
			result = region_tile::vector::unit(region_tile::direction::four);
		} else if (the_input().presses(SDLK_s)) {
			result = region_tile::vector::unit(region_tile::direction::five);
		} else if (the_input().presses(SDLK_d)) {
			result = region_tile::vector::unit(region_tile::direction::six);
		}
		if (result && _predicate(*result)) {
			return _cont(*result);
		}

		if (_origin && the_input().pressed(mouse_button::left)) {
			result = the_game().camera().tile_hovered() - *_origin;
			if (_predicate(*result)) {
				return _cont(*result);
			}
		}

		return state::open;
	}

	void vector_dialog::draw() const {
		draw_title(*_txt_title);
		draw_prompt(*_txt_prompt);

		// Highlight valid tiles.
		//! @todo This.
	}

	void vector_dialog::load_textures() {
		_txt_title = make_title(_title.c_str());
		_txt_prompt = make_prompt(_prompt.c_str());
	}
}
