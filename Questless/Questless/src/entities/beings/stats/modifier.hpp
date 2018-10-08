//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "being.hpp" //! @todo This could be forward-declared, but it would require practically doubling the amount of code in this file. Move inclusion to .cpp if ever added for another reason.

#include "utility/reference.hpp"
#include "utility/type_switch.hpp"
#include "utility/quantities.hpp"
#include "utility/utility.hpp"

#include <variant>
#include <vector>

namespace ql::stats {
	struct spirit_modifier { ql::mana amount; };
	struct regen_modifier { ql::per_tick amount; };
	struct strength_modifier { ql::strength amount; };
	struct toughness_modifier { ql::toughness amount; };
	struct stamina_modifier { ql::energy amount; };
	struct agility_modifier { ql::agility amount; };
	struct stealth_modifier { ql::stealth amount; };
	struct acuity_modifier { ql::acuity amount; };
	struct min_illuminance_modifier { ql::lum amount; };
	struct max_illuminance_modifier { ql::lum amount; };
	struct darkness_penalty_modifier { ql::acuity_per_lum amount; };
	struct glare_penalty_modifier { ql::acuity_per_lum amount; };
	struct hearing_modifier { ql::hearing amount; };
	struct intellect_modifier { ql::intellect amount; };
	struct speech_modifier { ql::speech amount; };
	struct mass_modifier { ql::load amount; };
	struct min_temp_modifier { ql::temperature amount; };
	struct max_temp_modifier { ql::temperature amount; };

	using modifier = std::variant
		< spirit_modifier
		, regen_modifier
		, strength_modifier
		, toughness_modifier
		, stamina_modifier
		, agility_modifier
		, stealth_modifier
		, acuity_modifier
		, min_illuminance_modifier
		, max_illuminance_modifier
		, darkness_penalty_modifier
		, glare_penalty_modifier
		, hearing_modifier
		, speech_modifier
		, intellect_modifier
		, mass_modifier
		, min_temp_modifier
		, max_temp_modifier
		>;

	//! Modifies @p stats according to @p modifier.
	inline void apply(modifier const& modifier, being& stats) {
		std::visit([&](auto&& mod) {
			SWITCH_TYPE(mod) {
				MATCH_TYPE(spirit_modifier) stats.a.spirit += mod.amount;
				MATCH_TYPE(regen_modifier) stats.regen += mod.amount;
				MATCH_TYPE(strength_modifier) stats.a.strength += mod.amount;
				MATCH_TYPE(toughness_modifier) stats.toughness += mod.amount;
				MATCH_TYPE(stamina_modifier) stats.a.stamina += mod.amount;
				MATCH_TYPE(agility_modifier) stats.a.agility += mod.amount;
				MATCH_TYPE(stealth_modifier) stats.stealth += mod.amount;
				MATCH_TYPE(acuity_modifier) stats.a.vision.acuity += mod.amount;
				MATCH_TYPE(min_illuminance_modifier) stats.a.vision.min_illuminance += mod.amount;
				MATCH_TYPE(max_illuminance_modifier) stats.a.vision.max_illuminance += mod.amount;
				MATCH_TYPE(darkness_penalty_modifier) stats.a.vision.darkness_penalty += mod.amount;
				MATCH_TYPE(glare_penalty_modifier) stats.a.vision.glare_penalty += mod.amount;
				MATCH_TYPE(hearing_modifier) stats.a.hearing += mod.amount;
				MATCH_TYPE(speech_modifier) stats.a.speech += mod.amount;
				MATCH_TYPE(intellect_modifier) stats.a.intellect += mod.amount;
				MATCH_TYPE(mass_modifier) stats.a.mass += mod.amount;
				MATCH_TYPE(min_temp_modifier) stats.min_temp += mod.amount;
				MATCH_TYPE(max_temp_modifier) stats.max_temp += mod.amount;
			}
		}, modifier);
	}

	//! Modifies @p stats according to @p modifiers.
	inline void apply_all(std::vector<modifier> const& modifiers, being& stats) {
		for (auto const& modifier : modifiers) {
			apply(modifier, stats);
		}
	}
}
