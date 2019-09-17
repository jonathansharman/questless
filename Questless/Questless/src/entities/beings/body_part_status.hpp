//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "bounded/nonnegative.hpp"
#include "damage/damage.hpp"
#include "entities/beings/stats/vision.hpp"
#include "quantities/game_time.hpp"
#include "quantities/misc.hpp"
#include "reg.hpp"

#include <optional>

namespace ql {
	enum class body_part_status_category { detrimental, neutral, beneficial };

	//! Causes healing over time.
	struct healing {
		static constexpr body_part_status_category category = body_part_status_category::beneficial;
		int additional_regen_factor;
		std::optional<id> o_source_id;
	};

	//! Deals blight damage over time.
	struct poisoned {
		static constexpr body_part_status_category category = body_part_status_category::detrimental;
		cancel::quotient_t<poisoning, tick> rate;
		std::optional<id> o_source_id;
	};

	//! A body part status modifier.
	using body_part_status = std::variant<healing, poisoned>;

	//! Retrieves the status category of @p status.
	auto get_category(body_part_status const& status) -> body_part_status_category;
}
