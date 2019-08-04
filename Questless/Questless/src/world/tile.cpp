//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "tile.hpp"

namespace ql {
	void make_tile(id id, terrain terrain, temperature temperature, lum luminance) {
		reg.assign<ql::terrain>(id, terrain);
		reg.assign<ql::temperature>(id, temperature);
		reg.assign<lum>(id, luminance);
		reg.assign<inventory>(id);
	}
}
