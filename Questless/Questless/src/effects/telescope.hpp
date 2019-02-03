//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "utility/id.hpp"
#include "world/coordinates.hpp"

namespace ql {
	struct being;

	namespace effects {
		struct telescope {
			region_tile::point origin;
			id<being> const caster_id;

			constexpr span range() const {
				return 3_span;
			}
		};
	}
}
