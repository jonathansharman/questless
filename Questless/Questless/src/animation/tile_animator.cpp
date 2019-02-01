//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "tile_animator.hpp"

#include "still_image.hpp"

#include "rsrc/tile.hpp"

namespace ql {
	tile_animator::~tile_animator() = default;

	void tile_animator::visit(dirt_tile const&) {
		animation = umake<still_image>(resources.txtr.dirt);
	}

	void tile_animator::visit(edge_tile const&) {
		//! @todo Edge tiles should simply not be drawn.
		animation = umake<still_image>(resources.txtr.blank);
	}

	void tile_animator::visit(grass_tile const&) {
		animation = umake<still_image>(resources.txtr.grass);
	}

	void tile_animator::visit(sand_tile const&) {
		animation = umake<still_image>(resources.txtr.sand);
	}

	void tile_animator::visit(snow_tile const&) {
		animation = umake<still_image>(resources.txtr.snow);
	}

	void tile_animator::visit(stone_tile const&) {
		animation = umake<still_image>(resources.txtr.stone);
	}

	void tile_animator::visit(water_tile const&) {
		animation = umake<still_image>(resources.txtr.water);
	}
}
