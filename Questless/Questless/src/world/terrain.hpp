//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

namespace ql {
	//! Type of terrain on a tile.
	enum class terrain : int { dirt = 0, edge, grass, sand, snow, stone, water, terrain_count };
}
