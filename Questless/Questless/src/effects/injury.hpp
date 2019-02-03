//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "damage/group.hpp"
#include "utility/id.hpp"
#include "world/coordinates.hpp"

#include <optional>

namespace ql {
	struct being;
	struct body_part;

	namespace effects {
		struct injury {
			region_tile::point origin;
			dmg::group const damage;
			id<being> target_being_id;
			id<body_part> target_part_id;
			std::optional<id<being>> opt_source_id;

			constexpr span range() const {
				return 7_span;
			}
		};
	}
}
