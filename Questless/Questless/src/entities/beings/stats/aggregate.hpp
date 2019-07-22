//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "vision.hpp"

#include "bounded/dynamic_nonnegative.hpp"
#include "quantities/mass.hpp"
#include "quantities/misc.hpp"

namespace ql::stats {
	//! The stats of a being that are based on aggregating stats of different body parts.
	struct aggregate {
		dynamic_nonnegative<health> vitality = {0_hp, 0_hp};
		nonnegative<mana> spirit = 0_mp;
		nonnegative<strength> strength = 0.0_str;
		nonnegative<energy> stamina = 0.0_ep;
		nonnegative<hearing> hearing = 0.0_hear;
		nonnegative<speech> speech = 0.0_speech;
		nonnegative<intellect> intellect = 0.0_int;
		nonnegative<mass> mass = 0.0_mass;
		nonnegative<undeath> undeath = 0_undeath;
		std::vector<vision> vision_sources{};

		//! The maximum volume of blood that can be stored in something with these stats.
		blood max_blood() const {
			constexpr auto max_blood_per_vitality = 1.0_blood / 1_hp;
			return max_blood_per_vitality * vitality.upper_bound();
		}

		//! The maximum rate at which something with these stats can lose blood.
		blood_per_tick max_bleeding() const {
			return max_blood() / 10_tick;
		}

		aggregate& operator+=(aggregate const& that) {
			vitality += that.vitality;
			spirit += that.spirit;
			strength += that.strength;
			stamina += that.stamina;
			hearing += that.hearing;
			speech += that.speech;
			intellect += that.intellect;
			mass += that.mass;
			vision_sources.insert(vision_sources.end(), that.vision_sources.begin(), that.vision_sources.end());
			return *this;
		}

		aggregate operator+(aggregate const& that) {
			aggregate result = *this;
			result += that;
			return result;
		}
	};
}
