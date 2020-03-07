//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "world/coordinates.hpp"

namespace ql::effects {
	struct arrow_attack {
		tile_hex_point origin;
		tile_hex_point const target;

		constexpr auto range() const -> pace {
			return 2 * (target - origin).length();
		}
	};
}
