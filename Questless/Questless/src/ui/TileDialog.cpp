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
			return _cont(std::nullopt);
		}

		if (_origin) {
			if (input().presses(SDLK_e)) {
				return _cont(_origin->neighbor(RegionTileCoords::Direction::one));
			} else if (input().presses(SDLK_w)) {
				return _cont(_origin->neighbor(RegionTileCoords::Direction::two));
			} else if (input().presses(SDLK_q)) {
				return _cont(_origin->neighbor(RegionTileCoords::Direction::three));
			} else if (input().presses(SDLK_a)) {
				return _cont(_origin->neighbor(RegionTileCoords::Direction::four));
			} else if (input().presses(SDLK_s)) {
				return _cont(_origin->neighbor(RegionTileCoords::Direction::five));
			} else if (input().presses(SDLK_d)) {
				return _cont(_origin->neighbor(RegionTileCoords::Direction::six));
			}
		}

		if (input().pressed(MouseButton::left)) {
			RegionTileCoords value = game().camera().tile_hovered();
			if (_predicate(value)) {
				return _cont(value);
			}
		}
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
