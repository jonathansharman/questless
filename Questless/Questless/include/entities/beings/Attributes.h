/**
* @file    Being.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Attributes struct, which holds a Being's attributes.
*/

#ifndef ATTRIBUTES_H
#define ATTRIBUTES_H

#include "utility/constants.h"

// Takes a namespace that contains constants for each of the attributes and applies it to each of the attributes in the same order as the constructor parameters.
#define ATTRIBUTE_PARAMS(NS) NS::vitality, NS::spirit, NS::health_regen, NS::mana_regen, NS::strength, NS::endurance, NS::stamina, NS::agility, NS::dexterity, NS::stealth, NS::vision, NS::light_affinity, \
NS::hearing, NS::intellect, NS::lift, NS::min_temp, NS::max_temp, NS::hands, NS::mute, NS::white_power, NS::black_power, NS::green_power, NS::red_power, NS::blue_power, NS::yellow_power, \
NS::white_resistance, NS::black_resistance, NS::green_resistance, NS::red_resistance, NS::blue_resistance, NS::yellow_resistance

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
		class VisionModifier : public ScalarModifier
		{
		public:
			VisionModifier(double magnitude) : ScalarModifier(magnitude) {}
		private:
			double& attribute(Attributes& attributes) override { return attributes.vision; }
		};
		class LightAffinityModifier : public ScalarModifier
		{
		public:
			LightAffinityModifier(double magnitude) : ScalarModifier(magnitude) {}
		private:
			double& attribute(Attributes& attributes) override { return attributes.light_affinity; }
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
			double& attribute(Attributes& attributes) override { return attributes.white_power; }
		};
		class BlackPowerModifier : public ScalarModifier
		{
		public:
			BlackPowerModifier(double magnitude) : ScalarModifier(magnitude) {}
		private:
			double& attribute(Attributes& attributes) override { return attributes.black_power; }
		};
		class GreenPowerModifier : public ScalarModifier
		{
		public:
			GreenPowerModifier(double magnitude) : ScalarModifier(magnitude) {}
		private:
			double& attribute(Attributes& attributes) override { return attributes.green_power; }
		};
		class RedPowerModifier : public ScalarModifier
		{
		public:
			RedPowerModifier(double magnitude) : ScalarModifier(magnitude) {}
		private:
			double& attribute(Attributes& attributes) override { return attributes.red_power; }
		};
		class BluePowerModifier : public ScalarModifier
		{
		public:
			BluePowerModifier(double magnitude) : ScalarModifier(magnitude) {}
		private:
			double& attribute(Attributes& attributes) override { return attributes.blue_power; }
		};
		class YellowPowerModifier : public ScalarModifier
		{
		public:
			YellowPowerModifier(double magnitude) : ScalarModifier(magnitude) {}
		private:
			double& attribute(Attributes& attributes) override { return attributes.yellow_power; }
		};

		// Magic Resistance Modifiers

		class WhiteResistanceModifier : public ScalarModifier
		{
		public:
			WhiteResistanceModifier(double magnitude) : ScalarModifier(magnitude) {}
		private:
			double& attribute(Attributes& attributes) override { return attributes.white_resistance; }
		};
		class BlackResistanceModifier : public ScalarModifier
		{
		public:
			BlackResistanceModifier(double magnitude) : ScalarModifier(magnitude) {}
		private:
			double& attribute(Attributes& attributes) override { return attributes.black_resistance; }
		};
		class GreenResistanceModifier : public ScalarModifier
		{
		public:
			GreenResistanceModifier(double magnitude) : ScalarModifier(magnitude) {}
		private:
			double& attribute(Attributes& attributes) override { return attributes.green_resistance; }
		};
		class RedResistanceModifier : public ScalarModifier
		{
		public:
			RedResistanceModifier(double magnitude) : ScalarModifier(magnitude) {}
		private:
			double& attribute(Attributes& attributes) override { return attributes.red_resistance; }
		};
		class BlueResistanceModifier : public ScalarModifier
		{
		public:
			BlueResistanceModifier(double magnitude) : ScalarModifier(magnitude) {}
		private:
			double& attribute(Attributes& attributes) override { return attributes.blue_resistance; }
		};
		class YellowResistanceModifier : public ScalarModifier
		{
		public:
			YellowResistanceModifier(double magnitude) : ScalarModifier(magnitude) {}
		private:
			double& attribute(Attributes& attributes) override { return attributes.yellow_resistance; }
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
		double vision;
		double light_affinity;
		double hearing;
		double intellect;
		double lift;
		double min_temp;
		double max_temp;
		bool mute;

		double white_power;
		double black_power;
		double green_power;
		double red_power;
		double blue_power;
		double yellow_power;

		double white_resistance;
		double black_resistance;
		double green_resistance;
		double red_resistance;
		double blue_resistance;
		double yellow_resistance;

		///////////////////////
		// Attribute Methods //
		///////////////////////

		/// @return The base attributes of the given Being subclass.
		template <typename BeingType>
		static Attributes of()
		{
			using t = BeingType;
			return Attributes{t::base_vitality, t::base_spirit, t::base_health_regen, t::base_mana_regen, t::base_strength, t::base_endurance, t::base_stamina, t::base_agility, t::base_dexterity,
				t::base_stealth, t::base_vision, t::base_light_affinity, t::base_hearing, t::base_intellect, t::base_lift, t::base_min_temp, t::base_max_temp, t::base_mute,
				t::base_white_power, t::base_black_power, t::base_green_power, t::base_red_power, t::base_blue_power, t::base_yellow_power, t::base_white_resistance,
				t::base_black_resistance, t::base_green_resistance, t::base_red_resistance, t::base_blue_resistance, t::base_yellow_resistance};
		}

		Attributes() {}

		Attributes(double vitality, double spirit, double health_regen, double mana_regen, double strength, double endurance, double stamina, double agility, double dexterity,
			double stealth, double vision, double light_affinity, double hearing, double intellect, double lift, double min_temp, double max_temp, bool mute,
			double white_power, double black_power, double green_power, double red_power, double blue_power, double yellow_power, double white_resistance,
			double black_resistance, double green_resistance, double red_resistance, double blue_resistance, double yellow_resistance)
			: vitality{vitality}, spirit{spirit}, health_regen{health_regen}, mana_regen{mana_regen}, strength{strength}, endurance{endurance}, stamina{stamina}, agility{agility}, dexterity{dexterity},
			stealth{stealth}, vision{vision}, light_affinity{light_affinity}, hearing{hearing}, intellect{intellect}, lift{lift}, min_temp{min_temp}, max_temp{max_temp}, mute{mute},
			white_power{white_power}, black_power{black_power}, green_power{green_power}, red_power{red_power}, blue_power{blue_power}, yellow_power{yellow_power}, white_resistance{white_resistance},
			black_resistance{black_resistance}, green_resistance{green_resistance}, red_resistance{red_resistance}, blue_resistance{blue_resistance}, yellow_resistance{yellow_resistance}
		{}

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
			if (vision < 0.0) { vision = 0.0; }
			if (light_affinity < 0.0) { light_affinity = 0.0; }
			if (light_affinity < 0.0) { light_affinity = 0.0; }
			if (hearing < 0.0) { hearing = 0.0; }
			if (intellect < 0.0) { intellect = 0.0; }
			if (lift < 0.0) { lift = 0.0; }

			if (white_power < 0.0) { white_power = 0.0; }
			if (black_power < 0.0) { black_power = 0.0; }
			if (green_power < 0.0) { green_power = 0.0; }
			if (red_power < 0.0) { red_power = 0.0; }
			if (blue_power < 0.0) { blue_power = 0.0; }
			if (yellow_power < 0.0) { yellow_power = 0.0; }

			if (white_resistance < 0.0) { white_resistance = 0.0; }
			if (black_resistance < 0.0) { black_resistance = 0.0; }
			if (green_resistance < 0.0) { green_resistance = 0.0; }
			if (red_resistance < 0.0) { red_resistance = 0.0; }
			if (blue_resistance < 0.0) { blue_resistance = 0.0; }
			if (yellow_resistance < 0.0) { yellow_resistance = 0.0; }
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
