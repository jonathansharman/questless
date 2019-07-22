//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "world/coordinates.hpp"

namespace ql {
	namespace effects {
		struct telescope {
			region_tile::point origin;
			ent const caster_id;

			constexpr span range() const {
				return 3_span;
			}
		};
	}
}
