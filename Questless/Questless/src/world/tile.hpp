//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "terrain.hpp"

#include "items/inventory.hpp"
#include "quantities/misc.hpp"

namespace ql {
	//! Makes @p id a tile, a hexagonal region of the world.
	//! @param terrain The terrain on this tile.
	//! @param temperature The temperature at this tile.
	//! @param luminance The amount of light at this tile.
	void make_tile(ent id, terrain terrain, temperature temperature, lum luminance);
}
