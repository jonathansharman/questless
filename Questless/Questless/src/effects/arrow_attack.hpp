//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "world/coordinates.hpp"

namespace ql::effects {
	struct arrow_attack {
		region_tile::point origin;
		region_tile::point const target;

		constexpr span range() const {
			return 2 * (target - origin).length();
		}
	};
}
