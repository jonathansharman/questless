//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

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
		auto max_blood() const -> blood {
			return vitality.base * 1.0_blood / 1_hp;
		}

		//! The maximum rate at which something with these stats can lose blood.
		auto max_bleeding() const -> blood_per_tick {
			return max_blood() / 10_tick;
		}

		//! Combines current values with those of @p that.
		auto combine_with(aggregate const& that) -> void {
			vitality.cur += that.vitality.cur;
			spirit.cur += that.spirit.cur;
			strength.cur += that.strength.cur;
			stamina.cur += that.stamina.cur;
			hearing.cur += that.hearing.cur;
			speech.cur += that.speech.cur;
			intellect.cur += that.intellect.cur;
			mass.cur += that.mass.cur;
			vision_sources.cur.insert(
				vision_sources.cur.end(), that.vision_sources.cur.begin(), that.vision_sources.cur.end());
		}

		//! Resets current stats to base stats.
		auto reset() -> void {
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
