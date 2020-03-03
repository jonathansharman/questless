//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "reg.hpp"
#include "stats/body.hpp"
#include "utility/visitation.hpp"

#include <variant>

namespace ql::generators {
	namespace human {
		auto make_body_stats() -> stats::body;

		struct torso {
			auto make(reg& reg, id owner_id) const -> id;
		};
		struct head {
			auto make(reg& reg, id owner_id) const -> id;
		};
		struct left_arm {
			auto make(reg& reg, id owner_id) const -> id;
		};
		struct right_arm {
			auto make(reg& reg, id owner_id) const -> id;
		};
		struct left_hand {
			auto make(reg& reg, id owner_id) const -> id;
		};
		struct right_hand {
			auto make(reg& reg, id owner_id) const -> id;
		};
		struct left_leg {
			auto make(reg& reg, id owner_id) const -> id;
		};
		struct right_leg {
			auto make(reg& reg, id owner_id) const -> id;
		};
		struct left_foot {
			auto make(reg& reg, id owner_id) const -> id;
		};
		struct right_foot {
			auto make(reg& reg, id owner_id) const -> id;
		};
	}

	//! Sum type for all default body part generators.
	struct generator {
		using value_t = std::variant< //
			human::torso,
			human::head,
			human::left_arm,
			human::right_arm,
			human::left_hand,
			human::right_hand,
			human::left_leg,
			human::right_leg,
			human::left_foot,
			human::right_foot>;

		value_t value;

		//! Generates a body part.
		//! @param owner_id The ID of the being to own the new body part.
		//! @return The ID of the new body part.
		auto make(reg& reg, id owner_id) const -> id;
	};
}
