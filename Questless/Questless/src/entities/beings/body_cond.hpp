//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

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
		constexpr bool weary() const {
			return energy.value() < min_energy + (max_energy - min_energy) / 4;
		}
		constexpr bool energized() const {
			return energy.value() > min_energy + 3 * (max_energy - min_energy) / 4;
		}

		static constexpr auto min_satiety = 0.0_sat;
		static constexpr auto max_satiety = 100.0_sat;
		static_bounded<ql::satiety, min_satiety, max_satiety> satiety = max_satiety;
		constexpr bool hungry() const {
			return satiety.value() < min_satiety + (max_satiety - min_satiety) / 4;
		}
		constexpr bool full() const {
			return satiety.value() > min_satiety + 3 * (max_satiety - min_satiety) / 4;
		}
		constexpr bool starving() const {
			return satiety.value() == min_satiety;
		}

		static constexpr auto min_alertness = 0.0_alert;
		static constexpr auto max_alertness = 100.0_alert;
		static_bounded<ql::alertness, min_alertness, max_alertness> alertness = max_alertness;
		constexpr bool sleepy() const {
			return alertness.value() < min_alertness + (max_alertness - min_alertness) / 4;
		}
		constexpr bool alert() const {
			return alertness.value() > min_alertness + 3 * (max_alertness - min_alertness) / 4;
		}

		static constexpr auto min_joy = -100.0_joy;
		static constexpr auto max_joy = 100.0_joy;
		static_bounded<ql::joy, min_joy, max_joy> joy = 0.0_joy;
		constexpr bool sad() const {
			return joy.value() < min_joy + (max_joy - min_joy) / 4;
		}
		constexpr bool happy() const {
			return joy.value() > min_joy + 3 * (max_joy - min_joy) / 4;
		}

		static constexpr auto min_courage = -100.0_courage;
		static constexpr auto max_courage = 100.0_courage;
		static_bounded<ql::courage, min_courage, max_courage> courage = 0.0_courage;
		constexpr bool afraid() const {
			return courage.value() < min_courage + (max_courage - min_courage) / 4;
		}
		constexpr bool brave() const {
			return courage.value() > min_courage + 3 * (max_courage - min_courage) / 4;
		}

		static constexpr auto min_serenity = -100.0_serenity;
		static constexpr auto max_serenity = 100.0_serenity;
		static_bounded<ql::serenity, min_serenity, max_serenity> serenity = 0.0_serenity;
		constexpr bool angry() const {
			return serenity.value() < min_serenity + (max_serenity - min_serenity) / 4;
		}
		constexpr bool calm() const {
			return serenity.value() > min_serenity + 3 * (max_serenity - min_serenity) / 4;
		}

		tile_hex::direction direction = tile_hex::direction::zero;

		ql::awakeness awakeness = awakeness::awake;
		constexpr bool awake() const {
			return awakeness == awakeness::awake;
		}
		constexpr bool asleep() const {
			return awakeness == awakeness::asleep;
		}

		ql::blood blood = 0.0_blood;

		ql::mortality mortality = ql::mortality::alive;
	};
}
