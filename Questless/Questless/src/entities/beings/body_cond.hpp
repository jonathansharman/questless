//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "bounded/nonnegative.hpp"
#include "bounded/static.hpp"
#include "quantities/misc.hpp"
#include "reg.hpp"
#include "world/coordinates.hpp"

namespace ql {
	enum class mortality : int { alive = 0, dead, undead, immortal };

	//! Encapsulates the temporary conditions of a body.
	struct body_cond {
		static constexpr auto min_energy = 0_ep;
		static constexpr auto max_energy = 100_ep;
		static_bounded<ql::energy, min_energy, max_energy> energy;
		constexpr auto weary() const -> bool {
			return energy.value() < min_energy + (max_energy - min_energy) / 4;
		}
		constexpr auto energized() const -> bool {
			return energy.value() > min_energy + 3 * (max_energy - min_energy) / 4;
		}

		static constexpr auto min_satiety = 0.0_sat;
		static constexpr auto max_satiety = 100.0_sat;
		static_bounded<ql::satiety, min_satiety, max_satiety> satiety = max_satiety;
		constexpr auto hungry() const -> bool {
			return satiety.value() < min_satiety + (max_satiety - min_satiety) / 4;
		}
		constexpr auto full() const -> bool {
			return satiety.value() > min_satiety + 3 * (max_satiety - min_satiety) / 4;
		}
		constexpr auto starving() const -> bool {
			return satiety.value() == min_satiety;
		}

		static constexpr auto min_alertness = 0.0_alert;
		static constexpr auto max_alertness = 100.0_alert;
		static_bounded<ql::alertness, min_alertness, max_alertness> alertness = max_alertness;
		constexpr auto sleepy() const -> bool {
			return alertness.value() < min_alertness + (max_alertness - min_alertness) / 4;
		}
		constexpr auto alert() const -> bool {
			return alertness.value() > min_alertness + 3 * (max_alertness - min_alertness) / 4;
		}

		static constexpr auto min_joy = -100.0_joy;
		static constexpr auto max_joy = 100.0_joy;
		static_bounded<ql::joy, min_joy, max_joy> joy = 0.0_joy;
		constexpr auto sad() const -> bool {
			return joy.value() < min_joy + (max_joy - min_joy) / 4;
		}
		constexpr auto happy() const -> bool {
			return joy.value() > min_joy + 3 * (max_joy - min_joy) / 4;
		}

		static constexpr auto min_courage = -100.0_courage;
		static constexpr auto max_courage = 100.0_courage;
		static_bounded<ql::courage, min_courage, max_courage> courage = 0.0_courage;
		constexpr auto afraid() const -> bool {
			return courage.value() < min_courage + (max_courage - min_courage) / 4;
		}
		constexpr auto brave() const -> bool {
			return courage.value() > min_courage + 3 * (max_courage - min_courage) / 4;
		}

		static constexpr auto min_serenity = -100.0_serenity;
		static constexpr auto max_serenity = 100.0_serenity;
		static_bounded<ql::serenity, min_serenity, max_serenity> serenity = 0.0_serenity;
		constexpr auto angry() const -> bool {
			return serenity.value() < min_serenity + (max_serenity - min_serenity) / 4;
		}
		constexpr auto calm() const -> bool {
			return serenity.value() > min_serenity + 3 * (max_serenity - min_serenity) / 4;
		}

		hex_direction direction = hex_direction::dr;

		ql::awakeness awakeness = awakeness::awake;
		constexpr auto awake() const -> bool {
			return awakeness == awakeness::awake;
		}
		constexpr auto asleep() const -> bool {
			return awakeness == awakeness::asleep;
		}

		ql::blood blood = 0.0_blood;

		ql::mortality mortality = ql::mortality::alive;
	};
}
