//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "ui/VectorDialog.h"
#include "Game.h"

using namespace sdl;
using namespace units;

namespace questless
{
	Dialog::State VectorDialog::update()
	{
		if (input().presses(SDLK_BACKSPACE) || input().presses(SDLK_ESCAPE)) {
			return _cont(std::nullopt);
		}

		std::optional<RegionTile::Vector> result;

		if (input().presses(SDLK_e)) {
			result = RegionTile::Vector::unit(RegionTile::Direction::one);
		} else if (input().presses(SDLK_w)) {
			result = RegionTile::Vector::unit(RegionTile::Direction::two);
		} else if (input().presses(SDLK_q)) {
			result = RegionTile::Vector::unit(RegionTile::Direction::three);
		} else if (input().presses(SDLK_a)) {
			result = RegionTile::Vector::unit(RegionTile::Direction::four);
		} else if (input().presses(SDLK_s)) {
			result = RegionTile::Vector::unit(RegionTile::Direction::five);
		} else if (input().presses(SDLK_d)) {
			result = RegionTile::Vector::unit(RegionTile::Direction::six);
		}
		if (result && _predicate(*result)) {
			return _cont(*result);
		}

		if (_origin && input().pressed(MouseButton::left)) {
			result = game().camera().tile_hovered() - *_origin;
			if (_predicate(*result)) {
				return _cont(*result);
			}
		}

		return State::open;
	}

	void VectorDialog::draw() const
	{
		draw_title(*_txt_title);
		draw_prompt(*_txt_prompt);

		// Highlight valid tiles.
		//! @todo This.
	}

	void VectorDialog::load_textures()
	{
		_txt_title = make_title(_title.c_str());
		_txt_prompt = make_prompt(_prompt.c_str());
	}
}
