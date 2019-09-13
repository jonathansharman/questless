//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "world/coordinates.hpp"

namespace ql::effects {
	struct arrow_attack {
		tile_hex::point origin;
		tile_hex::point const target;

		constexpr auto range() const -> span {
			return 2 * (target - origin).length();
		}
	};
}
