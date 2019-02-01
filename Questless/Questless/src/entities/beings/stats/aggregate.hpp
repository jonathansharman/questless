//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "vision.hpp"

#include "quantities/mass.hpp"
#include "quantities/quantities.hpp"

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
		nonnegative<speech> speech = 0.0_speech;
		nonnegative<intellect> intellect = 0.0_int;
		nonnegative<mass> mass = 0.0_mass;
		vision vision{};

		aggregate() = default;

		aggregate(ql::health vitality,
			ql::mana spirit,
			ql::strength strength,
			ql::energy stamina,
			ql::agility agility,
			ql::hearing hearing,
			ql::speech speech,
			ql::intellect intellect,
			ql::mass mass,
			stats::vision vision)
			: vitality{vitality}
			, spirit{spirit}
			, strength{strength}
			, stamina{stamina}
			, agility{agility}
			, hearing{hearing}
			, speech{speech}
			, intellect{intellect}
			, mass{mass}
			, vision{std::move(vision)} {}

		template <typename Archive>
		void save(Archive& archive) const {
			archive(CEREAL_NVP(vitality),
				CEREAL_NVP(spirit),
				CEREAL_NVP(strength),
				CEREAL_NVP(stamina),
				CEREAL_NVP(agility),
				CEREAL_NVP(vision),
				CEREAL_NVP(hearing),
				CEREAL_NVP(speech),
				CEREAL_NVP(intellect),
				CEREAL_NVP(mass),
				CEREAL_NVP(vision));
		}

		template <typename Archive>
		void load(Archive& archive) {
			archive(CEREAL_NVP(vitality),
				CEREAL_NVP(spirit),
				CEREAL_NVP(strength),
				CEREAL_NVP(stamina),
				CEREAL_NVP(agility),
				CEREAL_NVP(vision),
				CEREAL_NVP(hearing),
				CEREAL_NVP(speech),
				CEREAL_NVP(intellect),
				CEREAL_NVP(mass),
				CEREAL_NVP(vision));
		}

		aggregate& operator+=(aggregate const& that) {
			vitality += that.vitality;
			spirit += that.spirit;
			strength += that.strength;
			stamina += that.stamina;
			agility += that.agility;
			hearing += that.hearing;
			speech += that.speech;
			intellect += that.intellect;
			mass += that.mass;
			vision += that.vision;
			return *this;
		}

		aggregate operator+(aggregate const& that) {
			aggregate result = *this;
			result += that;
			return result;
		}
	};
}
