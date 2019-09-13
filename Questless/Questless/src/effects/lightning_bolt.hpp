//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "world/coordinates.hpp"

namespace ql::effects {
	struct lightning_bolt {
		tile_hex::point origin;

		constexpr auto range() const -> span {
			return 10_span;
		}
	};
}
