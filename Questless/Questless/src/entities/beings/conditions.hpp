//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "stats/being.hpp"

#include "quantities/misc.hpp"
#include "utility/dynamic_property.hpp"

namespace ql {
	enum class mortality : int { alive = 0, dead, undead, immortal };

	//! Encapsulates the temporary conditions of a being.
	struct conditions {
		stats::being& stats;

		lazy_bounded<ql::energy> energy;
		constexpr bool weary() const {
			return energy.value() < 0.5 * stats.a.stamina.value();
		}

		static constexpr auto min_satiety = 0.0_sat;
		static constexpr auto max_satiety = 100.0_sat;
		static_bounded<ql::satiety, min_satiety, max_satiety> satiety = max_satiety;
		constexpr bool hungry() const {
			return satiety.value() < min_satiety + 0.5 * (max_satiety - min_satiety);
		}
		constexpr bool starving() const {
			return satiety.value() == min_satiety;
		}

		static constexpr auto min_alertness = 0.0_alert;
		static constexpr auto max_alertness = 100.0_alert;
		static_bounded<ql::alertness, min_alertness, max_alertness> alertness = max_alertness;
		constexpr bool sleepy() const {
			return alertness.value() < min_alertness + 0.5 * (max_alertness - min_alertness);
		}

		static constexpr auto min_joy = -100.0_joy;
		static constexpr auto max_joy = 100.0_joy;
		static_bounded<ql::joy, min_joy, max_joy> joy = 0.0_joy;
		constexpr bool sad() const {
			return joy.value() < min_joy + 0.25 * (max_joy - min_joy);
		}
		constexpr bool happy() const {
			return joy.value() > min_joy + 0.75 * (max_joy - min_joy);
		}

		static constexpr auto min_courage = -100.0_courage;
		static constexpr auto max_courage = 100.0_courage;
		static_bounded<ql::courage, min_courage, max_courage> courage = 0.0_courage;
		constexpr bool afraid() const {
			return courage.value() < min_courage + 0.25 * (max_courage - min_courage);
		}
		constexpr bool brave() const {
			return courage.value() > min_courage + 0.75 * (max_courage - min_courage);
		}

		static constexpr auto min_serenity = -100.0_serenity;
		static constexpr auto max_serenity = 100.0_serenity;
		static_bounded<ql::serenity, min_serenity, max_serenity> serenity = 0.0_serenity;
		constexpr bool angry() const {
			return serenity.value() < min_serenity + 0.25 * (max_serenity - min_serenity);
		}
		constexpr bool calm() const {
			return serenity.value() > min_serenity + 0.75 * (max_serenity - min_serenity);
		}

		dynamic_property<ql::tick> busy_time = 0_tick;

		mortality mortality = mortality::alive;

		region_tile::direction direction;

		awakeness awakeness = awakeness::awake;
		constexpr bool awake() const {
			return awakeness == awakeness::awake;
		}
		constexpr bool asleep() const {
			return awakeness == awakeness::asleep;
		}

		corporeality corporeality = corporeality::corporeal;
		constexpr bool corporeal() const {
			return corporeality == corporeality::corporeal;
		}
		constexpr bool incorporeal() const {
			return corporeality == corporeality::incorporeal;
		}
	};
}
