//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "ui/TileDialog.h"
#include "Game.h"

using namespace sdl;
using namespace units;

namespace questless
{
	Dialog::State TileDialog::update()
	{
		if (input().presses(SDLK_BACKSPACE) || input().presses(SDLK_ESCAPE)) {
			game().world_renderer().clear_highlight_predicate();
			return _cont(std::nullopt);
		}

		std::optional<RegionTile::Point> result;

		if (_origin) {
			if (input().presses(SDLK_e)) {
				result = _origin->neighbor(RegionTile::Direction::one);
			} else if (input().presses(SDLK_w)) {
				result = _origin->neighbor(RegionTile::Direction::two);
			} else if (input().presses(SDLK_q)) {
				result = _origin->neighbor(RegionTile::Direction::three);
			} else if (input().presses(SDLK_a)) {
				result = _origin->neighbor(RegionTile::Direction::four);
			} else if (input().presses(SDLK_s)) {
				result = _origin->neighbor(RegionTile::Direction::five);
			} else if (input().presses(SDLK_d)) {
				result = _origin->neighbor(RegionTile::Direction::six);
			}
			if (result && _predicate(*result)) {
				game().world_renderer().clear_highlight_predicate();
				return _cont(*result);
			}
		}

		if (input().pressed(MouseButton::left)) {
			result = game().camera().tile_hovered();
			if (_predicate(*result)) {
				game().world_renderer().clear_highlight_predicate();
				return _cont(*result);
			}
		}

		game().world_renderer().set_highlight_predicate(_predicate);
		return State::open;
	}

	void TileDialog::draw() const
	{
		draw_title(*_txt_title);
		draw_prompt(*_txt_prompt);

		// Highlight valid tiles.
		//! @todo This.
	}

	void TileDialog::load_textures()
	{
		_txt_title = make_title(_title.c_str());
		_txt_prompt = make_prompt(_prompt.c_str());
	}
}
