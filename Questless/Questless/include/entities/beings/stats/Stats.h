/**
* @file    Stats.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Stats type, which holds a Being's stats.
*/

#ifndef ATTRIBUTES_H
#define ATTRIBUTES_H

#include <vector>
#include <memory>

#include "Vision.h"
#include "MagicStats.h"
#include "entities/beings/Protection.h"
#include "entities/beings/DamageMultiplier.h"
#include "utility/Property.h"

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

	class Stats
	{
	private:
		static void nonnegative_mutator(double& value, const double& new_value)
		{
			value = std::max(new_value, 0.0);
		}
	public:
		Property<double, nonnegative_mutator> vitality = 0.0;
		Property<double, nonnegative_mutator> spirit = 0.0;
		Property<double, nonnegative_mutator> health_regen = 0.0;
		Property<double, nonnegative_mutator> mana_regen = 0.0;
		Property<double, nonnegative_mutator> strength = 0.0;
		Property<double, nonnegative_mutator> endurance = 0.0;
		Property<double, nonnegative_mutator> stamina = 0.0;
		Property<double, nonnegative_mutator> agility = 0.0;
		Property<double, nonnegative_mutator> dexterity = 0.0;
		Property<double, nonnegative_mutator> stealth = 0.0;
		Vision vision;
		Property<double, nonnegative_mutator> hearing = 0.0;
		Property<double, nonnegative_mutator> intellect = 0.0;
		double weight = 0.0;
		Property<double, nonnegative_mutator> min_temp = 0.0;
		Property<double, nonnegative_mutator> max_temp = 0.0;
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

		friend std::ostream& operator <<(std::ostream& out, const Stats& a)
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

#endif
