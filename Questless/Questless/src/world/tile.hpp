//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "coordinates.hpp"
#include "terrain.hpp"

#include "items/inventory.hpp"
#include "quantities/misc.hpp"

namespace ql {
	//! Makes @p id a tile, a hexagonal region of the world.
	//! @param terrain The terrain on this tile.
	//! @param location This tile's location in the world.
	//! @param temperature The temperature at this tile.
	//! @param luminance The amount of light at this tile.
	void make_tile(id id, terrain terrain, location location, temperature temperature, lum luminance);
}
