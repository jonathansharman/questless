//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "world/coordinates.hpp"

namespace ql {
	namespace effects {
		struct telescope {
			tile_hex::point origin;
			id const caster_id;

			constexpr span range() const {
				return 3_span;
			}
		};
	}
}
