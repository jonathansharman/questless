//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "damage/group.hpp"
#include "world/coordinates.hpp"

#include <optional>

namespace ql {
	struct being;
	struct body_part;

	namespace effects {
		struct injury {
			region_tile::point origin;
			dmg::group const damage;
			ent target_being_id;
			ent target_part_id;
			std::optional<ent> opt_source_id;

			constexpr span range() const {
				return 7_span;
			}
		};
	}
}
