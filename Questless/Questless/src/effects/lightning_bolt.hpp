//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "world/coordinates.hpp"

namespace ql::effects {
	struct lightning_bolt {
		region_tile::point origin;

		constexpr span range() const {
			return 10_span;
		}
	};
}
