/**
* @file    Attributes.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Attributes struct, which holds a Being's attributes.
*/

#ifndef ATTRIBUTES_H
#define ATTRIBUTES_H

#include <vector>
#include <memory>

#include "Vision.h"
#include "MagicAttribute.h"
#include "entities/beings/Protection.h"
#include "entities/beings/DamageMultiplier.h"

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
	struct Lift : TaggedType<double> { using TaggedType::TaggedType; };
	struct MinTemp : TaggedType<double> { using TaggedType::TaggedType; };
	struct MaxTemp : TaggedType<double> { using TaggedType::TaggedType; };
	struct Mute : TaggedType<bool> { using TaggedType::TaggedType; };

	struct Attributes
	{
		double vitality = 0.0;
		double spirit = 0.0;
		double health_regen = 0.0;
		double mana_regen = 0.0;
		double strength = 0.0;
		double endurance = 0.0;
		double stamina = 0.0;
		double agility = 0.0;
		double dexterity = 0.0;
		double stealth = 0.0;
		Vision vision;
		double hearing = 0.0;
		double intellect = 0.0;
		double lift = 0.0;
		double min_temp = 0.0;
		double max_temp = 0.0;
		bool mute = false;

		Protection protection = Protection::zero();
		Resistance resistance = Resistance::zero();
		Vulnerability vulnerability = Vulnerability::zero();

		MagicPower magic_power = MagicPower::zero();
		MagicResistance magic_resistance = MagicResistance::zero();

		/// @return The base attributes of the given Being subclass.
		template <typename BeingType>
		static constexpr Attributes of() { return BeingType::base_attributes; }

		constexpr Attributes() {}

		constexpr Attributes
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
			, Lift lift
			, MinTemp min_temp
			, MaxTemp max_temp
			, Mute mute
			, Protection protection
			, Resistance resistance
			, Vulnerability vulnerability
			, MagicPower magic_power
			, MagicResistance magic_resistance
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
			, lift{std::move(lift)}
			, min_temp{std::move(min_temp)}
			, max_temp{std::move(max_temp)}
			, mute{std::move(mute)}
			, protection{std::move(protection)}
			, resistance{std::move(resistance)}
			, vulnerability{std::move(vulnerability)}
			, magic_power{std::move(magic_power)}
			, magic_resistance{std::move(magic_resistance)}
		{}

		friend std::ostream& operator <<(std::ostream& out, const Attributes& a)
		{
			out << a.vitality << ' ' << a.spirit << ' ' << a.health_regen << ' ' << a.mana_regen << ' ' << a.strength << ' ' << a.endurance << ' '
				<< a.stamina << ' ' << a.agility << ' ' << a.dexterity << ' ' << a.stealth << ' ' << a.vision << ' ' << a.hearing << ' ' << a.intellect
				<< ' ' << a.lift << ' ' << a.min_temp << ' ' << a.max_temp << ' ' << a.mute << ' ' << a.magic_power << ' ' << a.magic_resistance << ' ';
			return out;
		}

		friend std::istream& operator >>(std::istream& in, Attributes& a)
		{
			in >> a.vitality >> a.spirit >> a.health_regen >> a.mana_regen >> a.strength >> a.endurance >> a.stamina >> a.agility >> a.dexterity
				>> a.stealth >> a.vision >> a.hearing >> a.intellect >> a.lift >> a.min_temp >> a.max_temp >> a.mute >> a.magic_power >> a.magic_resistance;
			return in;
		}

		/// @todo Eliminate clamping by moving fields to private and writing setters.

		/// Sets all attributes that are below their minimum values to their minimum values.
		void clamp()
		{	
			if (vitality < 0.0) { vitality = 0.0; }
			if (spirit < 0.0) { spirit = 0.0; }
			if (health_regen < 0.0) { health_regen = 0.0; }
			if (mana_regen < 0.0) { mana_regen = 0.0; }
			if (strength < 0.0) { strength = 0.0; }
			if (endurance < 0.0) { endurance = 0.0; }
			if (stamina < 0.0) { stamina = 0.0; }
			if (agility < 0.0) { agility = 0.0; }
			if (dexterity < 0.0) { dexterity = 0.0; }
			if (stealth < 0.0) { stealth = 0.0; }
			if (hearing < 0.0) { hearing = 0.0; }
			if (intellect < 0.0) { intellect = 0.0; }
			if (lift < 0.0) { lift = 0.0; }
		}
	};
}

#endif
