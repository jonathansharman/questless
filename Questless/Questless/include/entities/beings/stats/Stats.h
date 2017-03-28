/**
* @file    Stats.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*/

#pragma once

#include <vector>
#include <memory>

#include "Vision.h"
#include "MagicStats.h"
#include "entities/beings/Protection.h"
#include "entities/beings/DamageMultiplier.h"
#include "utility/Bounded.h"

namespace questless
{
	struct Vitality : TaggedType<double> { using TaggedType::TaggedType; };
	struct Spirit : TaggedType<double> { using TaggedType::TaggedType; };
	struct HealthRegen : TaggedType<double> { using TaggedType::TaggedType; };
	struct ManaRegen : TaggedType<double> { using TaggedType::TaggedType; };
	struct Strength : TaggedType<double> { using TaggedType::TaggedType; };
	struct Endurance : TaggedType<double> { using TaggedType::TaggedType; };
	struct Stamina : TaggedType<double> { using TaggedType::TaggedType; };
	struct Agility : TaggedType<double> { using TaggedType::TaggedType; };
	struct Dexterity : TaggedType<double> { using TaggedType::TaggedType; };
	struct Stealth : TaggedType<double> { using TaggedType::TaggedType; };
	struct Hearing : TaggedType<double> { using TaggedType::TaggedType; };
	struct Intellect : TaggedType<double> { using TaggedType::TaggedType; };
	struct Weight : TaggedType<double> { using TaggedType::TaggedType; };
	struct MinTemp : TaggedType<double> { using TaggedType::TaggedType; };
	struct MaxTemp : TaggedType<double> { using TaggedType::TaggedType; };
	struct Mute : TaggedType<bool> { using TaggedType::TaggedType; };

	/// Represents the stats of a being.
	////
	struct Stats
	{
		static constexpr double minimum_value = 0.0;

		Bounded<double, minimum_value> vitality = 0.0;
		Bounded<double, minimum_value> spirit = 0.0;
		Bounded<double, minimum_value> health_regen = 0.0;
		Bounded<double, minimum_value> mana_regen = 0.0;
		Bounded<double, minimum_value> strength = 0.0;
		Bounded<double, minimum_value> endurance = 0.0;
		Bounded<double, minimum_value> stamina = 0.0;
		Bounded<double, minimum_value> agility = 0.0;
		Bounded<double, minimum_value> dexterity = 0.0;
		Bounded<double, minimum_value> stealth = 0.0;
		Vision vision;
		Bounded<double, minimum_value> hearing = 0.0;
		Bounded<double, minimum_value> intellect = 0.0;
		double weight = 0.0;
		Bounded<double, minimum_value> min_temp = 0.0;
		Bounded<double, minimum_value> max_temp = 0.0;
		bool mute = false;

		Protection protection = Protection::zero();
		Resistance resistance = Resistance::zero();
		Vulnerability vulnerability = Vulnerability::zero();

		Magic magic = Magic::zero();
		Antimagic antimagic = Antimagic::zero();

		constexpr Stats() {}

		constexpr Stats
			( Vitality vitality
			, Spirit spirit
			, HealthRegen health_regen
			, ManaRegen mana_regen
			, Strength strength
			, Endurance endurance
			, Stamina stamina
			, Agility agility
			, Dexterity dexterity
			, Stealth stealth
			, Vision vision
			, Hearing hearing
			, Intellect intellect
			, Weight weight
			, MinTemp min_temp
			, MaxTemp max_temp
			, Mute mute
			, Protection protection
			, Resistance resistance
			, Vulnerability vulnerability
			, Magic magic
			, Antimagic antimagic
			)
			: vitality{std::move(vitality)}
			, spirit{std::move(spirit)}
			, health_regen{std::move(health_regen)}
			, mana_regen{std::move(mana_regen)}
			, strength{std::move(strength)}
			, endurance{std::move(endurance)}
			, stamina{std::move(stamina)}
			, agility{std::move(agility)}
			, dexterity{std::move(dexterity)}
			, stealth{std::move(stealth)}
			, vision{std::move(vision)}
			, hearing{std::move(hearing)}
			, intellect{std::move(intellect)}
			, weight{std::move(weight)}
			, min_temp{std::move(min_temp)}
			, max_temp{std::move(max_temp)}
			, mute{std::move(mute)}
			, protection{std::move(protection)}
			, resistance{std::move(resistance)}
			, vulnerability{std::move(vulnerability)}
			, magic{std::move(magic)}
			, antimagic{std::move(antimagic)}
		{}

		friend std::ostream& operator <<(std::ostream& out, Stats const& a)
		{
			out << a.vitality << ' ' << a.spirit << ' ' << a.health_regen << ' ' << a.mana_regen << ' ' << a.strength << ' ' << a.endurance << ' '
				<< a.stamina << ' ' << a.agility << ' ' << a.dexterity << ' ' << a.stealth << ' ' << a.vision << ' ' << a.hearing << ' '
				<< a.intellect << ' ' << a.weight << ' ' << a.min_temp << ' ' << a.max_temp << ' ' << a.mute << ' ' << a.magic << ' ' << a.antimagic << ' ';
			return out;
		}

		friend std::istream& operator >>(std::istream& in, Stats& a)
		{
			in >> a.vitality >> a.spirit >> a.health_regen >> a.mana_regen >> a.strength >> a.endurance >> a.stamina >> a.agility >> a.dexterity
				>> a.stealth >> a.vision >> a.hearing >> a.intellect >> a.weight >> a.min_temp >> a.max_temp >> a.mute >> a.magic >> a.antimagic;
			return in;
		}
	};
}
