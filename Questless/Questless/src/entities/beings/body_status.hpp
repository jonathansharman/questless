//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "bounded/nonnegative.hpp"
#include "damage/damage.hpp"
#include "entities/beings/stats/vision.hpp"
#include "quantities/game_time.hpp"
#include "quantities/misc.hpp"
#include "reg.hpp"

#include <optional>

namespace ql {
	enum class body_status_category { detrimental, neutral, beneficial };

	//! Decreases visual acuity.
	struct blind {
		static constexpr body_status_category category = body_status_category::detrimental;
		perception acuity_reduction;
		std::optional<id> o_source_id;
	};

	//! Reduces intellect.
	struct confused {
		static constexpr body_status_category category = body_status_category::detrimental;
		intellect reduction;
		std::optional<id> o_source_id;
	};

	//! Reduces hearing.
	struct deaf {
		static constexpr body_status_category category = body_status_category::detrimental;
		hearing reduction;
		std::optional<id> o_source_id;
	};

	//! Reduces speech.
	struct mute {
		static constexpr body_status_category category = body_status_category::detrimental;
		speech reduction;
		std::optional<id> o_source_id;
	};

	//! Reduces strength and max energy.
	struct nauseated {
		static constexpr body_status_category category = body_status_category::detrimental;
		using nausea = cancel::quantity<int, cancel::unit_t<struct nausea_tag>>;
		static constexpr auto strength_penalty = 1_str / nausea{1};
		static constexpr auto max_energy_penalty = 1_ep / nausea{1};

		nausea magnitude;
		std::optional<id> o_source_id;
	};

	//! Reduces action points.
	struct numb {
		static constexpr body_status_category category = body_status_category::detrimental;
		using numbness = cancel::quantity<int, cancel::unit_t<struct numbness_tag>>;
		static constexpr auto ability_penalty = 1_ap / numbness{1};

		numbness magnitude;
		std::optional<id> o_source_id;
	};

	//! Increases visual acuity.
	struct telescoped {
		static constexpr body_status_category category = body_status_category::beneficial;
		perception acuity_bonus;
		std::optional<id> o_source_id;
	};

	//! A body status modifier.
	using body_status = std::variant<blind, confused, deaf, mute, nauseated, numb, telescoped>;

	//! Retrieves the status category of @p status.
	body_status_category get_category(body_status const& status);
}
