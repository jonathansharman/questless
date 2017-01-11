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
#include "MagicPower.h"
#include "MagicResistance.h"
#include "entities/beings/Protection.h"
#include "entities/beings/DamageMultiplier.h"

namespace questless
{
	struct Attributes
	{
		double vitality;
		double spirit;
		double health_regen;
		double mana_regen;
		double strength;
		double endurance;
		double stamina;
		double agility;
		double dexterity;
		double stealth;
		Vision vision;
		double hearing;
		double intellect;
		double lift;
		double min_temp;
		double max_temp;
		bool mute;

		Protection protection;
		Resistance resistance;
		Vulnerability vulnerability;

		MagicPower magic_power;
		MagicResistance magic_resistance;

		/// @return The base attributes of the given Being subclass.
		template <typename BeingType>
		static Attributes of()
		{
			using T = BeingType;
			return Attributes{T::base_vitality, T::base_spirit, T::base_health_regen, T::base_mana_regen, T::base_strength, T::base_endurance, T::base_stamina,
				T::base_agility, T::base_dexterity, T::base_stealth, Vision::of<T>(), T::base_hearing, T::base_intellect, T::base_lift, T::base_min_temp,
				T::base_max_temp, T::base_mute, Protection::of<T>(), Resistance::of<T>(), Vulnerability::of<T>(), MagicPower::of<T>(), MagicResistance::of<T>()};
		}

		Attributes() {}

		Attributes(double vitality, double spirit, double health_regen, double mana_regen, double strength, double endurance, double stamina,
			double agility, double dexterity, double stealth, Vision vision, double hearing, double intellect, double lift, double min_temp, double max_temp,
			bool mute, Protection protection, Resistance resistance, Vulnerability vulnerability, MagicPower magic_power, MagicResistance magic_resistance)
			: vitality{vitality}, spirit{spirit}, health_regen{health_regen}, mana_regen{mana_regen}, strength{strength}, endurance{endurance}, stamina{stamina},
			agility{agility}, dexterity{dexterity}, stealth{stealth}, vision{vision}, hearing{hearing}, intellect{intellect}, lift{lift}, min_temp{min_temp}, max_temp{max_temp},
			mute{mute}, protection{protection}, resistance{resistance}, vulnerability{vulnerability}, magic_power{magic_power}, magic_resistance{magic_resistance}
		{}

		friend std::ostream& operator <<(std::ostream& out, const Attributes& a)
		{
			out << a.vitality << ' ' << a.spirit << ' ' << a.health_regen << ' ' << a.mana_regen << ' ' << a.strength << ' ' << a.endurance << ' '
				<< a.stamina << ' ' << a.agility << ' ' << a.dexterity << ' ' << a.stealth << ' ' << a.vision << ' ' << a.hearing << ' ' << a.intellect
				<< ' ' << a.lift << ' ' << a.min_temp << ' ' << a.max_temp << ' ' << a.mute << ' ' << a.magic_power << ' ' << a.magic_resistance;
			return out;
		}

		friend std::istream& operator >>(std::istream& in, Attributes& a)
		{
			in >> a.vitality >> a.spirit >> a.health_regen >> a.mana_regen >> a.strength >> a.endurance >> a.stamina >> a.agility >> a.dexterity
				>> a.stealth >> a.vision >> a.hearing >> a.intellect >> a.lift >> a.min_temp >> a.max_temp >> a.mute >> a.magic_power >> a.magic_resistance;
			return in;
		}

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
			vision.clamp();
			if (hearing < 0.0) { hearing = 0.0; }
			if (intellect < 0.0) { intellect = 0.0; }
			if (lift < 0.0) { lift = 0.0; }

			magic_power.clamp();

			magic_resistance.clamp();
		}
	};
}

#endif
