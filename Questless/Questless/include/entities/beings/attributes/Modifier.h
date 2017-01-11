/**
* @file    Attributes.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Attributes struct, which holds a Being's attributes.
*/

#ifndef MODIFIER_H
#define MODIFIER_H

#include <memory>
#include <vector>

#include "Attributes.h"

namespace questless
{
	class Modifier
	{
	public:
		using ptr = std::unique_ptr<Modifier>;

		/// Makes a vector of modifiers from the given modifier.
		static std::vector<ptr> make_vector(ptr modifier)
		{
			std::vector<ptr> modifiers;
			modifiers.push_back(std::move(modifier));
			return modifiers;
		}

		/// Makes a vector of modifiers from the given non-empty variadic list of modifiers.
		template <typename... RestModifiers, typename LastModifier>
		static std::vector<ptr> make_vector(RestModifiers... rest, LastModifier last)
		{
			std::vector<ptr> modifiers = make_vector(std::forward<RestModifiers...>(rest));
			modifiers.push_back(last);
			return modifiers;
		}

		/// Modifies the attribute according to the given modifiers.
		/// @param modifiers A vector of attribute modifiers.
		static void apply_all(const std::vector<Modifier::ptr>& modifiers, Attributes& attributes)
		{
			for (const auto& modifier : modifiers) {
				modifier->apply(attributes);
			}
		}

		virtual void apply(Attributes& attributes) = 0;
	};

	/// Overrides whether the being is mute.
	class MuteModifier : public Modifier
	{
	public:
		MuteModifier(bool mute) : _mute{mute} {}
		static ptr make(bool mute) { return std::make_unique<MuteModifier>(mute); }
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
		static ptr make(double magnitude) { return std::make_unique<VitalityModifier>(magnitude); }
	private:
		double& attribute(Attributes& attributes) override { return attributes.vitality; }
	};
	class SpiritModifier : public ScalarModifier
	{
	public:
		SpiritModifier(double magnitude) : ScalarModifier(magnitude) {}
		static ptr make(double magnitude) { return std::make_unique<SpiritModifier>(magnitude); }
	private:
		double& attribute(Attributes& attributes) override { return attributes.spirit; }
	};
	class HealthRegenModifier : public ScalarModifier
	{
	public:
		HealthRegenModifier(double magnitude) : ScalarModifier(magnitude) {}
		static ptr make(double magnitude) { return std::make_unique<HealthRegenModifier>(magnitude); }
	private:
		double& attribute(Attributes& attributes) override { return attributes.health_regen; }
	};
	class ManaRegenModifier : public ScalarModifier
	{
	public:
		ManaRegenModifier(double magnitude) : ScalarModifier(magnitude) {}
		static ptr make(double magnitude) { return std::make_unique<ManaRegenModifier>(magnitude); }
	private:
		double& attribute(Attributes& attributes) override { return attributes.mana_regen; }
	};
	class StrengthModifier : public ScalarModifier
	{
	public:
		StrengthModifier(double magnitude) : ScalarModifier(magnitude) {}
		static ptr make(double magnitude) { return std::make_unique<StrengthModifier>(magnitude); }
	private:
		double& attribute(Attributes& attributes) override { return attributes.strength; }
	};
	class EnduranceModifier : public ScalarModifier
	{
	public:
		EnduranceModifier(double magnitude) : ScalarModifier(magnitude) {}
		static ptr make(double magnitude) { return std::make_unique<EnduranceModifier>(magnitude); }
	private:
		double& attribute(Attributes& attributes) override { return attributes.endurance; }
	};
	class StaminaModifier : public ScalarModifier
	{
	public:
		StaminaModifier(double magnitude) : ScalarModifier(magnitude) {}
		static ptr make(double magnitude) { return std::make_unique<StaminaModifier>(magnitude); }
	private:
		double& attribute(Attributes& attributes) override { return attributes.stamina; }
	};
	class AgilityModifier : public ScalarModifier
	{
	public:
		AgilityModifier(double magnitude) : ScalarModifier(magnitude) {}
		static ptr make(double magnitude) { return std::make_unique<AgilityModifier>(magnitude); }
	private:
		double& attribute(Attributes& attributes) override { return attributes.agility; }
	};
	class DexterityModifier : public ScalarModifier
	{
	public:
		DexterityModifier(double magnitude) : ScalarModifier(magnitude) {}
		static ptr make(double magnitude) { return std::make_unique<DexterityModifier>(magnitude); }
	private:
		double& attribute(Attributes& attributes) override { return attributes.dexterity; }
	};
	class StealthModifier : public ScalarModifier
	{
	public:
		StealthModifier(double magnitude) : ScalarModifier(magnitude) {}
		static ptr make(double magnitude) { return std::make_unique<StealthModifier>(magnitude); }
	private:
		double& attribute(Attributes& attributes) override { return attributes.stealth; }
	};
	class VisualAcuityModifier : public ScalarModifier
	{
	public:
		VisualAcuityModifier(double magnitude) : ScalarModifier(magnitude) {}
		static ptr make(double magnitude) { return std::make_unique<VisualAcuityModifier>(magnitude); }
	private:
		double& attribute(Attributes& attributes) override { return attributes.vision.acuity; }
	};
	class IdealLightModifier : public ScalarModifier
	{
	public:
		IdealLightModifier(double magnitude) : ScalarModifier(magnitude) {}
		static ptr make(double magnitude) { return std::make_unique<IdealLightModifier>(magnitude); }
	private:
		double& attribute(Attributes& attributes) override { return attributes.vision.ideal_light; }
	};
	class LightToleranceModifier : public ScalarModifier
	{
	public:
		LightToleranceModifier(double magnitude) : ScalarModifier(magnitude) {}
		static ptr make(double magnitude) { return std::make_unique<LightToleranceModifier>(magnitude); }
	private:
		double& attribute(Attributes& attributes) override { return attributes.vision.light_tolerance; }
	};
	class HearingModifier : public ScalarModifier
	{
	public:
		HearingModifier(double magnitude) : ScalarModifier(magnitude) {}
		static ptr make(double magnitude) { return std::make_unique<HearingModifier>(magnitude); }
	private:
		double& attribute(Attributes& attributes) override { return attributes.hearing; }
	};
	class IntellectModifier : public ScalarModifier
	{
	public:
		IntellectModifier(double magnitude) : ScalarModifier(magnitude) {}
		static ptr make(double magnitude) { return std::make_unique<IntellectModifier>(magnitude); }
	private:
		double& attribute(Attributes& attributes) override { return attributes.intellect; }
	};
	class LiftModifier : public ScalarModifier
	{
	public:
		LiftModifier(double magnitude) : ScalarModifier(magnitude) {}
		static ptr make(double magnitude) { return std::make_unique<LiftModifier>(magnitude); }
	private:
		double& attribute(Attributes& attributes) override { return attributes.lift; }
	};
	class MinTempModifier : public ScalarModifier
	{
	public:
		MinTempModifier(double magnitude) : ScalarModifier(magnitude) {}
		static ptr make(double magnitude) { return std::make_unique<MinTempModifier>(magnitude); }
	private:
		double& attribute(Attributes& attributes) override { return attributes.min_temp; }
	};
	class MaxTempModifier : public ScalarModifier
	{
	public:
		MaxTempModifier(double magnitude) : ScalarModifier(magnitude) {}
		static ptr make(double magnitude) { return std::make_unique<MaxTempModifier>(magnitude); }
	private:
		double& attribute(Attributes& attributes) override { return attributes.max_temp; }
	};

	// Magic Power Modifers

	class WhitePowerModifier : public ScalarModifier
	{
	public:
		WhitePowerModifier(double magnitude) : ScalarModifier(magnitude) {}
		static ptr make(double magnitude) { return std::make_unique<WhitePowerModifier>(magnitude); }
	private:
		double& attribute(Attributes& attributes) override { return attributes.magic_power.white; }
	};
	class BlackPowerModifier : public ScalarModifier
	{
	public:
		BlackPowerModifier(double magnitude) : ScalarModifier(magnitude) {}
		static ptr make(double magnitude) { return std::make_unique<BlackPowerModifier>(magnitude); }
	private:
		double& attribute(Attributes& attributes) override { return attributes.magic_power.black; }
	};
	class GreenPowerModifier : public ScalarModifier
	{
	public:
		GreenPowerModifier(double magnitude) : ScalarModifier(magnitude) {}
		static ptr make(double magnitude) { return std::make_unique<GreenPowerModifier>(magnitude); }
	private:
		double& attribute(Attributes& attributes) override { return attributes.magic_power.green; }
	};
	class RedPowerModifier : public ScalarModifier
	{
	public:
		RedPowerModifier(double magnitude) : ScalarModifier(magnitude) {}
		static ptr make(double magnitude) { return std::make_unique<RedPowerModifier>(magnitude); }
	private:
		double& attribute(Attributes& attributes) override { return attributes.magic_power.red; }
	};
	class BluePowerModifier : public ScalarModifier
	{
	public:
		BluePowerModifier(double magnitude) : ScalarModifier(magnitude) {}
		static ptr make(double magnitude) { return std::make_unique<BluePowerModifier>(magnitude); }
	private:
		double& attribute(Attributes& attributes) override { return attributes.magic_power.blue; }
	};
	class YellowPowerModifier : public ScalarModifier
	{
	public:
		YellowPowerModifier(double magnitude) : ScalarModifier(magnitude) {}
		static ptr make(double magnitude) { return std::make_unique<YellowPowerModifier>(magnitude); }
	private:
		double& attribute(Attributes& attributes) override { return attributes.magic_power.yellow; }
	};

	// Magic Resistance Modifiers

	class WhiteResistanceModifier : public ScalarModifier
	{
	public:
		WhiteResistanceModifier(double magnitude) : ScalarModifier(magnitude) {}
		static ptr make(double magnitude) { return std::make_unique<WhiteResistanceModifier>(magnitude); }
	private:
		double& attribute(Attributes& attributes) override { return attributes.magic_resistance.white; }
	};
	class BlackResistanceModifier : public ScalarModifier
	{
	public:
		BlackResistanceModifier(double magnitude) : ScalarModifier(magnitude) {}
		static ptr make(double magnitude) { return std::make_unique<BlackResistanceModifier>(magnitude); }
	private:
		double& attribute(Attributes& attributes) override { return attributes.magic_resistance.black; }
	};
	class GreenResistanceModifier : public ScalarModifier
	{
	public:
		GreenResistanceModifier(double magnitude) : ScalarModifier(magnitude) {}
		static ptr make(double magnitude) { return std::make_unique<GreenResistanceModifier>(magnitude); }
	private:
		double& attribute(Attributes& attributes) override { return attributes.magic_resistance.green; }
	};
	class RedResistanceModifier : public ScalarModifier
	{
	public:
		RedResistanceModifier(double magnitude) : ScalarModifier(magnitude) {}
		static ptr make(double magnitude) { return std::make_unique<RedResistanceModifier>(magnitude); }
	private:
		double& attribute(Attributes& attributes) override { return attributes.magic_resistance.red; }
	};
	class BlueResistanceModifier : public ScalarModifier
	{
	public:
		BlueResistanceModifier(double magnitude) : ScalarModifier(magnitude) {}
		static ptr make(double magnitude) { return std::make_unique<BlueResistanceModifier>(magnitude); }
	private:
		double& attribute(Attributes& attributes) override { return attributes.magic_resistance.blue; }
	};
	class YellowResistanceModifier : public ScalarModifier
	{
	public:
		YellowResistanceModifier(double magnitude) : ScalarModifier(magnitude) {}
		static ptr make(double magnitude) { return std::make_unique<YellowResistanceModifier>(magnitude); }
	private:
		double& attribute(Attributes& attributes) override { return attributes.magic_resistance.yellow; }
	};
}

#endif
