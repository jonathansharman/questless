//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "utility/visitor_pattern.hpp"

namespace ql {
	class dirt_tile;
	class edge_tile;
	class grass_tile;
	class sand_tile;
	class snow_tile;
	class stone_tile;
	class water_tile;

	using tile_subtype_list = cancel::list_t
		< dirt_tile
		, edge_tile
		, grass_tile
		, sand_tile
		, snow_tile
		, stone_tile
		, water_tile
		>;

	DEFINE_VISITORS(tile, tile_subtype_list)

	enum class tile_subtype : int
		{ dirt = 0
		, edge
		, grass
		, sand
		, snow
		, stone
		, water
		, TILE_SUBTYPE_COUNT //! @todo Remove TILE_SUBTYPE_COUNT if static reflection that can check enum length is ever added.
		};
}
