//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "quantities/misc.hpp"
#include "utility/id.hpp"

#include <variant>

namespace ql {
	struct dirt {};
	struct edge {};
	struct grass {};
	struct sand {};
	struct snow {};
	struct stone {};
	struct water {};

	//! Tile terrain types.
	struct terrain {
		std::variant<dirt, edge, grass, sand, snow, stone, water> value;
	};

	//! A hexagonal region of the world.
	struct tile {
		id<tile> const id;

		//! Offset from ambient temperature at this tile.
		ql::temperature temperature_offset;

		//! The terrain on this tile.
		terrain terrain;
	};
}
