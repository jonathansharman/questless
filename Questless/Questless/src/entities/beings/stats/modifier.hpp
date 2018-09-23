//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <memory>
#include <vector>

#include "being.hpp" //! @todo This could be forward-declared, but it would require practically doubling the amount of code in this file. Move inclusion to .cpp if ever added for another reason.

#include "utility/reference.hpp"

namespace ql::stats {
	//! A modification to a stat of a being.
	struct modifier {
		//! Modifies the stat according to the given modifiers.
		//! @param modifiers A vector of stat modifiers.
		static void apply_all(std::vector<uptr<modifier>> const& modifiers, being& stats) {
			for (auto const& modifier : modifiers) {
				modifier->apply(stats);
			}
		}

		virtual void apply(being& stats) = 0;
	};

	//! Overrides whether the being is mute.
	class mute_modifier : public modifier {
	public:
		constexpr mute_modifier(ql::mute mute) : _mute{mute} {}
		static auto make(ql::mute mute) { return umake<mute_modifier>(mute); }
		void apply(being& stats) final { stats.a.mute = _mute; }
	private:
		ql::mute _mute;
	};

	struct spirit_modifier : public modifier {
		ql::mana modifier;
		void apply(being& stats) final { stats.a.spirit += modifier; }
	};
	struct regen_modifier : public modifier {
		ql::per_tick modifier;
		void apply(being& stats) final { stats.regen += modifier; }
	};
	struct strength_modifier : public modifier {
		ql::strength modifier;
		void apply(being& stats) final { stats.a.strength += modifier; }
	};
	struct toughness_modifier : public modifier {
		ql::toughness modifier;
		void apply(being& stats) final { stats.toughness += modifier; }
	};
	struct stamina_modifier : public modifier {
		ql::energy modifier;
		void apply(being& stats) final { stats.a.stamina += modifier; }
	};
	struct agility_modifier : public modifier {
		ql::agility modifier;
		void apply(being& stats) final { stats.a.agility += modifier; }
	};
	struct stealth_modifier : public modifier {
		ql::stealth modifier;
		void apply(being& stats) final { stats.stealth += modifier; }
	};

	// Vision Modifiers

	struct visual_acuity_modifier : public modifier {
		ql::acuity modifier;
		void apply(being& stats) final { stats.a.vision.acuity += modifier; }
	};
	struct min_luminance_modifier : public modifier {
		ql::illuminance modifier;
		void apply(being& stats) final { stats.a.vision.min_illuminance += modifier; }
	};
	struct max_luminance_modifier : public modifier {
		ql::illuminance modifier;
		void apply(being& stats) final { stats.a.vision.max_illuminance += modifier; }
	};
	struct light_tolerance_modifier : public modifier {
		ql::darkness_tolerance modifier;
		void apply(being& stats) final { stats.a.vision.darkness_tolerance += modifier; }
	};

	struct hearing_modifier : public modifier {
		ql::hearing modifier;
		void apply(being& stats) final { stats.a.hearing += modifier; }
	};
	struct intellect_modifier : public modifier {
		ql::intellect modifier;
		void apply(being& stats) final { stats.a.intellect += modifier; }
	};
	struct weight_modifier : public modifier {
		ql::weight modifier;
		void apply(being& stats) final { stats.a.weight += modifier; }
	};
	struct min_temp_modifier : public modifier {
		ql::temperature modifier;
		void apply(being& stats) final { stats.min_temp += modifier; }
	};
	struct max_temp_modifier : public modifier {
		ql::temperature modifier;
		void apply(being& stats) final { stats.max_temp += modifier; }
	};
}
