//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "vision.hpp"

#include "utility/io.hpp"
#include "utility/quantities.hpp"

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
		nonnegative<weight> weight = 0.0_wt;
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
			, ql::weight weight
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
			, weight{weight}
			, mute{mute}
			, vision{std::move(vision)}
		{}

		aggregate& operator +=(aggregate const& that) {
			vitality += that.vitality;
			spirit += that.spirit;
			strength += that.strength;
			stamina += that.stamina;
			agility += that.agility;
			hearing += that.hearing;
			intellect += that.intellect;
			weight += that.weight;
			if (that.mute.value) mute = ql::mute{true};
			vision += that.vision;
		}

		aggregate operator +(aggregate const& that) {
			aggregate result = *this;
			result += that;
			return result;
		}
	};

	void to_json(nlohmann::json& j, aggregate const& a) {
		if (a.vitality != 0.0_hp) to_json(j["vitality"], a.vitality);
		if (a.spirit != 0.0_mp) to_json(j["spirit"], a.spirit);
		if (a.strength != 0.0_str) to_json(j["strength"], a.strength);
		if (a.stamina != 0.0_ep) to_json(j["stamina"], a.stamina);
		if (a.agility != 0.0_agi) to_json(j["agility"], a.agility);
		if (a.hearing != 0.0_hear) to_json(j["hearing"], a.hearing);
		if (a.intellect != 0.0_int) to_json(j["intellect"], a.intellect);
		if (a.weight != 0.0_wt) to_json(j["weight"], a.weight);
		to_json(j["mute"], a.mute);
		to_json(j["vision"], a.vision);
	}

	void from_json(nlohmann::json const& j, aggregate& a) {
		auto const vitality = j.find("vitality");
		if (vitality != j.end()) from_json(vitality.value(), a.vitality);

		auto const spirit = j.find("spirit");
		if (spirit != j.end()) from_json(spirit.value(), a.spirit);

		auto const strength = j.find("strength");
		if (strength != j.end()) from_json(strength.value(), a.strength);

		auto const stamina = j.find("stamina");
		if (stamina != j.end()) from_json(stamina.value(), a.stamina);

		auto const agility = j.find("agility");
		if (agility != j.end()) from_json(agility.value(), a.agility);

		auto const vision = j.find("vision");
		if (vision != j.end()) from_json(vitality.value(), a.vision);

		auto const hearing = j.find("hearing");
		if (hearing != j.end()) from_json(hearing.value(), a.hearing);

		auto const intellect = j.find("intellect");
		if (intellect != j.end()) from_json(intellect.value(), a.intellect);

		auto const weight = j.find("weight");
		if (weight != j.end()) from_json(weight.value(), a.weight);

		auto const mute = j.find("mute");
		if (mute != j.end()) from_json(mute.value(), a.mute);
	}
}
