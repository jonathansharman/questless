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
#include "utility/constants.h"

namespace questless
{
	struct Attributes
	{
		////////////////////////////////
		// Attribute Modifier Classes //
		////////////////////////////////

		class Modifier
		{
		public:
			virtual void apply(Attributes& attributes) = 0;
		};

		/// Overrides whether the being is mute.
		class MuteModifier : public Modifier
		{
		public:
			MuteModifier(bool mute) : _mute{mute} {}
			void apply(Attributes& attributes) override { attributes.mute = _mute; }
		private:
			bool _mute;
		};

		/// Adjusts a scalar attribute by some magnitude.
		class ScalarModifier : public Modifier
		{
		public:
			ScalarModifier(double magnitude) : _magnitude{magnitude} {}
			void apply(Attributes& attributes) override { attribute(attributes) += _magnitude; }
		private:
			double _magnitude;
			virtual double& attribute(Attributes& attributes) = 0;
		};
		class VitalityModifier : public ScalarModifier
		{
		public:
			VitalityModifier(double magnitude) : ScalarModifier(magnitude) {}
		private:
			double& attribute(Attributes& attributes) override { return attributes.vitality; }
		};
		class SpiritModifier : public ScalarModifier
		{
		public:
			SpiritModifier(double magnitude) : ScalarModifier(magnitude) {}
		private:
			double& attribute(Attributes& attributes) override { return attributes.spirit; }
		};
		class HealthRegenModifier : public ScalarModifier
		{
		public:
			HealthRegenModifier(double magnitude) : ScalarModifier(magnitude) {}
		private:
			double& attribute(Attributes& attributes) override { return attributes.health_regen; }
		};
		class ManaRegenModifier : public ScalarModifier
		{
		public:
			ManaRegenModifier(double magnitude) : ScalarModifier(magnitude) {}
		private:
			double& attribute(Attributes& attributes) override { return attributes.mana_regen; }
		};
		class StrengthModifier : public ScalarModifier
		{
		public:
			StrengthModifier(double magnitude) : ScalarModifier(magnitude) {}
		private:
			double& attribute(Attributes& attributes) override { return attributes.strength; }
		};
		class EnduranceModifier : public ScalarModifier
		{
		public:
			EnduranceModifier(double magnitude) : ScalarModifier(magnitude) {}
		private:
			double& attribute(Attributes& attributes) override { return attributes.endurance; }
		};
		class StaminaModifier : public ScalarModifier
		{
		public:
			StaminaModifier(double magnitude) : ScalarModifier(magnitude) {}
		private:
			double& attribute(Attributes& attributes) override { return attributes.stamina; }
		};
		class AgilityModifier : public ScalarModifier
		{
		public:
			AgilityModifier(double magnitude) : ScalarModifier(magnitude) {}
		private:
			double& attribute(Attributes& attributes) override { return attributes.agility; }
		};
		class DexterityModifier : public ScalarModifier
		{
		public:
			DexterityModifier(double magnitude) : ScalarModifier(magnitude) {}
		private:
			double& attribute(Attributes& attributes) override { return attributes.dexterity; }
		};
		class StealthModifier : public ScalarModifier
		{
		public:
			StealthModifier(double magnitude) : ScalarModifier(magnitude) {}
		private:
			double& attribute(Attributes& attributes) override { return attributes.stealth; }
		};
		class VisualAcuityModifier : public ScalarModifier
		{
		public:
			VisualAcuityModifier(double magnitude) : ScalarModifier(magnitude) {}
		private:
			double& attribute(Attributes& attributes) override { return attributes.vision.acuity; }
		};
		class IdealLightModifier : public ScalarModifier
		{
		public:
			IdealLightModifier(double magnitude) : ScalarModifier(magnitude) {}
		private:
			double& attribute(Attributes& attributes) override { return attributes.vision.ideal_light; }
		};
		class LightRangeModifier : public ScalarModifier
		{
		public:
			LightRangeModifier(double magnitude) : ScalarModifier(magnitude) {}
		private:
			double& attribute(Attributes& attributes) override { return attributes.vision.light_tolerance; }
		};
		class HearingModifier : public ScalarModifier
		{
		public:
			HearingModifier(double magnitude) : ScalarModifier(magnitude) {}
		private:
			double& attribute(Attributes& attributes) override { return attributes.hearing; }
		};
		class IntellectModifier : public ScalarModifier
		{
		public:
			IntellectModifier(double magnitude) : ScalarModifier(magnitude) {}
		private:
			double& attribute(Attributes& attributes) override { return attributes.intellect; }
		};
		class LiftModifier : public ScalarModifier
		{
		public:
			LiftModifier(double magnitude) : ScalarModifier(magnitude) {}
		private:
			double& attribute(Attributes& attributes) override { return attributes.lift; }
		};
		class MinTempModifier : public ScalarModifier
		{
		public:
			MinTempModifier(double magnitude) : ScalarModifier(magnitude) {}
		private:
			double& attribute(Attributes& attributes) override { return attributes.min_temp; }
		};
		class MaxTempModifier : public ScalarModifier
		{
		public:
			MaxTempModifier(double magnitude) : ScalarModifier(magnitude) {}
		private:
			double& attribute(Attributes& attributes) override { return attributes.max_temp; }
		};

		// Magic Power Modifers

		class WhitePowerModifier : public ScalarModifier
		{
		public:
			WhitePowerModifier(double magnitude) : ScalarModifier(magnitude) {}
		private:
			double& attribute(Attributes& attributes) override { return attributes.magic_power.white; }
		};
		class BlackPowerModifier : public ScalarModifier
		{
		public:
			BlackPowerModifier(double magnitude) : ScalarModifier(magnitude) {}
		private:
			double& attribute(Attributes& attributes) override { return attributes.magic_power.black; }
		};
		class GreenPowerModifier : public ScalarModifier
		{
		public:
			GreenPowerModifier(double magnitude) : ScalarModifier(magnitude) {}
		private:
			double& attribute(Attributes& attributes) override { return attributes.magic_power.green; }
		};
		class RedPowerModifier : public ScalarModifier
		{
		public:
			RedPowerModifier(double magnitude) : ScalarModifier(magnitude) {}
		private:
			double& attribute(Attributes& attributes) override { return attributes.magic_power.red; }
		};
		class BluePowerModifier : public ScalarModifier
		{
		public:
			BluePowerModifier(double magnitude) : ScalarModifier(magnitude) {}
		private:
			double& attribute(Attributes& attributes) override { return attributes.magic_power.blue; }
		};
		class YellowPowerModifier : public ScalarModifier
		{
		public:
			YellowPowerModifier(double magnitude) : ScalarModifier(magnitude) {}
		private:
			double& attribute(Attributes& attributes) override { return attributes.magic_power.yellow; }
		};

		// Magic Resistance Modifiers

		class WhiteResistanceModifier : public ScalarModifier
		{
		public:
			WhiteResistanceModifier(double magnitude) : ScalarModifier(magnitude) {}
		private:
			double& attribute(Attributes& attributes) override { return attributes.magic_resistance.white; }
		};
		class BlackResistanceModifier : public ScalarModifier
		{
		public:
			BlackResistanceModifier(double magnitude) : ScalarModifier(magnitude) {}
		private:
			double& attribute(Attributes& attributes) override { return attributes.magic_resistance.black; }
		};
		class GreenResistanceModifier : public ScalarModifier
		{
		public:
			GreenResistanceModifier(double magnitude) : ScalarModifier(magnitude) {}
		private:
			double& attribute(Attributes& attributes) override { return attributes.magic_resistance.green; }
		};
		class RedResistanceModifier : public ScalarModifier
		{
		public:
			RedResistanceModifier(double magnitude) : ScalarModifier(magnitude) {}
		private:
			double& attribute(Attributes& attributes) override { return attributes.magic_resistance.red; }
		};
		class BlueResistanceModifier : public ScalarModifier
		{
		public:
			BlueResistanceModifier(double magnitude) : ScalarModifier(magnitude) {}
		private:
			double& attribute(Attributes& attributes) override { return attributes.magic_resistance.blue; }
		};
		class YellowResistanceModifier : public ScalarModifier
		{
		public:
			YellowResistanceModifier(double magnitude) : ScalarModifier(magnitude) {}
		private:
			double& attribute(Attributes& attributes) override { return attributes.magic_resistance.yellow; }
		};

		//////////////////////
		// Attribute Fields //
		//////////////////////

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

		MagicPower magic_power;

		MagicResistance magic_resistance;

		///////////////////////
		// Attribute Methods //
		///////////////////////

		/// @return The base attributes of the given Being subclass.
		template <typename BeingType>
		static Attributes of()
		{
			using t = BeingType;
			return Attributes{t::base_vitality, t::base_spirit, t::base_health_regen, t::base_mana_regen, t::base_strength, t::base_endurance, t::base_stamina,
				t::base_agility, t::base_dexterity, t::base_stealth, Vision::of<t>(), t::base_hearing, t::base_intellect, t::base_lift,
				t::base_min_temp, t::base_max_temp, t::base_mute, MagicPower::of<t>(), MagicResistance::of<t>()};
		}

		Attributes() {}

		Attributes(double vitality, double spirit, double health_regen, double mana_regen, double strength, double endurance, double stamina,
			double agility, double dexterity, double stealth, Vision vision, double hearing, double intellect, double lift,
			double min_temp, double max_temp, bool mute, MagicPower magic_power, MagicResistance magic_resistance)
			: vitality{vitality}, spirit{spirit}, health_regen{health_regen}, mana_regen{mana_regen}, strength{strength}, endurance{endurance},
			stamina{stamina}, agility{agility}, dexterity{dexterity}, stealth{stealth}, vision{vision}, hearing{hearing}, intellect{intellect},
			lift{lift}, min_temp{min_temp}, max_temp{max_temp}, mute{mute}, magic_power{magic_power}, magic_resistance{magic_resistance}
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

		/// Modifies the attribute according to the given modifiers.
		/// @param modifiers A vector of attribute modifiers.
		void modify(const std::vector<std::unique_ptr<Attributes::Modifier>>& modifiers)
		{
			for (const auto& modifier : modifiers) {
				modifier->apply(*this);
			}
		}
	};
}

#endif
