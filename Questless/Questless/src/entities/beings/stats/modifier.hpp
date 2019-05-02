//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "being.hpp" //! @todo This could be forward-declared, but it would require practically doubling the amount of code in this file. Move inclusion to .cpp if ever added for another reason.

#include "quantities/misc.hpp"
#include "utility/reference.hpp"
#include "utility/utility.hpp"

#include <variant>
#include <vector>

namespace ql::stats {
	struct spirit_modifier {
		ql::mana amount;
	};
	struct regen_modifier {
		ql::per_tick amount;
	};
	struct strength_modifier {
		ql::strength amount;
	};
	struct stamina_modifier {
		ql::energy amount;
	};
	struct agility_modifier {
		ql::agility amount;
	};
	struct stealth_modifier {
		ql::stealth amount;
	};
	struct acuity_modifier {
		ql::acuity amount;
	};
	struct min_illuminance_modifier {
		ql::lum amount;
	};
	struct max_illuminance_modifier {
		ql::lum amount;
	};
	struct darkness_penalty_modifier {
		ql::acuity_per_lum amount;
	};
	struct glare_penalty_modifier {
		ql::acuity_per_lum amount;
	};
	struct hearing_modifier {
		ql::hearing amount;
	};
	struct intellect_modifier {
		ql::intellect amount;
	};
	struct speech_modifier {
		ql::speech amount;
	};
	struct mass_modifier {
		ql::mass amount;
	};
	struct min_temp_modifier {
		ql::temperature amount;
	};
	struct max_temp_modifier {
		ql::temperature amount;
	};

	using modifier = std::variant<spirit_modifier,
		regen_modifier,
		strength_modifier,
		stamina_modifier,
		agility_modifier,
		stealth_modifier,
		acuity_modifier,
		min_illuminance_modifier,
		max_illuminance_modifier,
		darkness_penalty_modifier,
		glare_penalty_modifier,
		hearing_modifier,
		speech_modifier,
		intellect_modifier,
		mass_modifier,
		min_temp_modifier,
		max_temp_modifier>;

	//! Modifies @p stats according to @p modifier.
	inline void apply(modifier const& modifier, being& stats) {
		match(modifier,
			[&](spirit_modifier const& mod) { stats.a.spirit += mod.amount; },
			[&](regen_modifier const& mod) { stats.regen += mod.amount; },
			[&](strength_modifier const& mod) { stats.a.strength += mod.amount; },
			[&](stamina_modifier const& mod) { stats.a.stamina += mod.amount; },
			[&](agility_modifier const& mod) { stats.a.agility += mod.amount; },
			[&](stealth_modifier mod) { stats.stealth += mod.amount; },
			[&](acuity_modifier const& mod) { stats.a.vision.acuity += mod.amount; },
			[&](min_illuminance_modifier const& mod) { stats.a.vision.min_illuminance += mod.amount; },
			[&](max_illuminance_modifier const& mod) { stats.a.vision.max_illuminance += mod.amount; },
			[&](darkness_penalty_modifier const& mod) { stats.a.vision.darkness_penalty += mod.amount; },
			[&](glare_penalty_modifier const& mod) { stats.a.vision.glare_penalty += mod.amount; },
			[&](hearing_modifier const& mod) { stats.a.hearing += mod.amount; },
			[&](speech_modifier const& mod) { stats.a.speech += mod.amount; },
			[&](intellect_modifier const& mod) { stats.a.intellect += mod.amount; },
			[&](mass_modifier const& mod) { stats.a.mass += mod.amount; },
			[&](min_temp_modifier const& mod) { stats.min_temp += mod.amount; },
			[&](max_temp_modifier const& mod) { stats.max_temp += mod.amount; });
	}

	//! Modifies @p stats according to @p modifiers.
	inline void apply_all(std::vector<modifier> const& modifiers, being& stats) {
		for (auto const& modifier : modifiers) {
			apply(modifier, stats);
		}
	}
}
