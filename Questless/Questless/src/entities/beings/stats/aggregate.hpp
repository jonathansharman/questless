//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "vision.hpp"

#include "utility/quantities.hpp"

#include <cereal/cereal.hpp>

namespace ql::stats {
	//! The stats of a being that are based on aggregating stats of different body parts.
	struct aggregate {
		nonnegative<health> vitality = 0.0_hp;
		nonnegative<mana> spirit = 0.0_mp;
		nonnegative<strength> strength = 0.0_str;
		nonnegative<energy> stamina = 0.0_ep;
		nonnegative<agility> agility = 0.0_agi;
		nonnegative<hearing> hearing = 0.0_hear;
		nonnegative<intellect> intellect = 0.0_int;
		nonnegative<load> mass = 0.0_load;
		mute mute{true};
		vision vision{};

		aggregate() = default;

		aggregate
			( ql::health vitality
			, ql::mana spirit
			, ql::strength strength
			, ql::energy stamina
			, ql::agility agility
			, ql::hearing hearing
			, ql::intellect intellect
			, ql::load mass
			, ql::mute mute
			, stats::vision vision
			)
			: vitality{vitality}
			, spirit{spirit}
			, strength{strength}
			, stamina{stamina}
			, agility{agility}
			, hearing{hearing}
			, intellect{intellect}
			, mass{mass}
			, mute{mute}
			, vision{std::move(vision)}
		{}

		template <typename Archive>
		void save(Archive& archive) const {
			archive
				( CEREAL_NVP(vitality)
				, CEREAL_NVP(spirit)
				, CEREAL_NVP(strength)
				, CEREAL_NVP(stamina)
				, CEREAL_NVP(agility)
				, CEREAL_NVP(vision)
				, CEREAL_NVP(hearing)
				, CEREAL_NVP(intellect)
				, CEREAL_NVP(mass)
				, CEREAL_NVP(mute)
				, CEREAL_NVP(vision)
				);
		}

		template <typename Archive>
		void load(Archive& archive) {
			archive
				( CEREAL_NVP(vitality)
				, CEREAL_NVP(spirit)
				, CEREAL_NVP(strength)
				, CEREAL_NVP(stamina)
				, CEREAL_NVP(agility)
				, CEREAL_NVP(vision)
				, CEREAL_NVP(hearing)
				, CEREAL_NVP(intellect)
				, CEREAL_NVP(mass)
				, CEREAL_NVP(mute)
				, CEREAL_NVP(vision)
				);
		}

		aggregate& operator +=(aggregate const& that) {
			vitality += that.vitality;
			spirit += that.spirit;
			strength += that.strength;
			stamina += that.stamina;
			agility += that.agility;
			hearing += that.hearing;
			intellect += that.intellect;
			mass += that.mass;
			if (that.mute.value) mute = ql::mute{true};
			vision += that.vision;
			return *this;
		}

		aggregate operator +(aggregate const& that) {
			aggregate result = *this;
			result += that;
			return result;
		}
	};
}
