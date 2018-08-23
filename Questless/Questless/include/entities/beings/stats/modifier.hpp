//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <memory>
#include <vector>

#include "stats.hpp" //! @todo This could be forward-declared, but it would require practically doubling the amount of code in this file. Move inclusion to .cpp if ever added for another reason.
#include "utility/reference.hpp"

namespace ql {
	//! A modification to a stat of a being.
	struct modifier {
		//! Modifies the stat according to the given modifiers.
		//! @param modifiers A vector of stat modifiers.
		static void apply_all(std::vector<uptr<modifier>> const& modifiers, stats& stats) {
			for (auto const& modifier : modifiers) {
				modifier->apply(stats);
			}
		}

		virtual void apply(stats& stats) = 0;
	};

	//! Overrides whether the being is mute.
	class mute_modifier : public modifier {
	public:
		constexpr mute_modifier(bool mute) : _mute{mute} {}
		static auto make(bool mute) { return umake<mute_modifier>(mute); }
		void apply(stats& stats) final { stats.mute = _mute; }
	private:
		bool _mute;
	};

	//! Adjusts a scalar stat by some magnitude.
	class scalar_modifier : public modifier {
	public:
		constexpr scalar_modifier(double magnitude) : _magnitude{magnitude} {}
		constexpr double magnitude() const { return _magnitude; }
	private:
		double _magnitude;
	};
	struct spirit_modifier : public scalar_modifier {
		using scalar_modifier::scalar_modifier;
		void apply(stats& stats) final { stats.spirit += magnitude(); }
	};
	struct health_regen_modifier : public scalar_modifier {
		using scalar_modifier::scalar_modifier;
		void apply(stats& stats) final { stats.health_regen += magnitude(); }
	};
	struct strength_modifier : public scalar_modifier {
		using scalar_modifier::scalar_modifier;
		void apply(stats& stats) final { stats.strength += magnitude(); }
	};
	struct endurance_modifier : public scalar_modifier {
		using scalar_modifier::scalar_modifier;
		void apply(stats& stats) final { stats.endurance += magnitude(); }
	};
	struct stamina_modifier : public scalar_modifier {
		using scalar_modifier::scalar_modifier;
		void apply(stats& stats) final { stats.stamina += magnitude(); }
	};
	struct agility_modifier : public scalar_modifier {
		using scalar_modifier::scalar_modifier;
		void apply(stats& stats) final { stats.agility += magnitude(); }
	};
	struct stealth_modifier : public scalar_modifier {
		using scalar_modifier::scalar_modifier;
		void apply(stats& stats) final { stats.stealth += magnitude(); }
	};

	// Vision Modifiers

	struct visual_acuity_modifier : public scalar_modifier {
		using scalar_modifier::scalar_modifier;
		void apply(stats& stats) final { stats.vision.acuity += magnitude(); }
	};
	struct ideal_luminance_modifier : public scalar_modifier {
		using scalar_modifier::scalar_modifier;
		void apply(stats& stats) final { stats.vision.ideal_illuminance += magnitude(); }
	};
	struct light_tolerance_modifier : public scalar_modifier {
		using scalar_modifier::scalar_modifier;
		void apply(stats& stats) final { stats.vision.darkness_tolerance += magnitude(); }
	};

	struct hearing_modifier : public scalar_modifier {
		using scalar_modifier::scalar_modifier;
		void apply(stats& stats) final { stats.hearing += magnitude(); }
	};
	struct intellect_modifier : public scalar_modifier {
		using scalar_modifier::scalar_modifier;
		void apply(stats& stats) final { stats.intellect += magnitude(); }
	};
	struct weight_modifier : public scalar_modifier {
		using scalar_modifier::scalar_modifier;
		void apply(stats& stats) final { stats.weight += magnitude(); }
	};
	struct min_temp_modifier : public scalar_modifier {
		using scalar_modifier::scalar_modifier;
		void apply(stats& stats) final { stats.min_temp += magnitude(); }
	};
	struct max_temp_modifier : public scalar_modifier {
		using scalar_modifier::scalar_modifier;
		void apply(stats& stats) final { stats.max_temp += magnitude(); }
	};
}
