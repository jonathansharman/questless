//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "damage/group.hpp"
#include "world/coordinates.hpp"

#include <optional>

namespace ql {
	struct body_part;

	namespace effects {
		struct injury {
			tile_hex::point origin;
			dmg::group const damage;
			id target_being_id;
			id target_part_id;
			std::optional<id> o_source_id;

			constexpr auto range() const -> pace {
				return 7_pace;
			}
		};
	}
}
