//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "tile.hpp"

namespace ql {
	auto make_tile(id tile_id, terrain terrain, location location, temperature temperature, lum luminance) -> id {
		reg.assign<ql::terrain>(tile_id, terrain);
		reg.assign<ql::location>(tile_id, location);
		reg.assign<ql::temperature>(tile_id, temperature);
		reg.assign<lum>(tile_id, luminance);
		reg.assign<inventory>(tile_id);

		return tile_id;
	}
}
