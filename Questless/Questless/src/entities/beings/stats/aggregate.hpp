//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "stat.hpp"
#include "vision.hpp"

#include "bounded/dynamic_nonnegative.hpp"
#include "quantities/mass.hpp"
#include "quantities/misc.hpp"

namespace ql::stats {
	//! The stats of a being that are based on aggregating stats of different body parts.
	struct aggregate {
		stat<health> vitality;
		stat<mana> spirit;
		stat<strength> strength;
		stat<energy> stamina;
		stat<hearing> hearing;
		stat<speech> speech;
		stat<intellect> intellect;
		stat<mass> mass;
		stat<undeath> undeath;
		stat<std::vector<vision>> vision_sources;

		//! The maximum volume of blood that can be stored in something with these stats.
		blood max_blood() const {
			return vitality.base * 1.0_blood / 1_hp;
		}

		//! The maximum rate at which something with these stats can lose blood.
		blood_per_tick max_bleeding() const {
			return max_blood() / 10_tick;
		}

		//! Combines this aggregate with @p that, per stat.
		void combine_with(aggregate const& that) {
			vitality.base += that.vitality.base;
			spirit.base += that.spirit.base;
			strength.base += that.strength.base;
			stamina.base += that.stamina.base;
			hearing.base += that.hearing.base;
			speech.base += that.speech.base;
			intellect.base += that.intellect.base;
			mass.base += that.mass.base;
			vision_sources.base.insert(
				vision_sources.base.end(), that.vision_sources.base.begin(), that.vision_sources.base.end());
		}

		//! Resets current stats to base stats.
		void reset() {
			vitality.reset();
			spirit.reset();
			strength.reset();
			stamina.reset();
			hearing.reset();
			speech.reset();
			intellect.reset();
			mass.reset();
			vision_sources.reset();
		}
	};
}
