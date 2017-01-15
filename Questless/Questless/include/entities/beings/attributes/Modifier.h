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
	struct Modifier
	{
		using ptr = std::unique_ptr<Modifier>;

		/// Makes a vector of modifiers from the given modifier.
		template <typename ModifierPtr>
		static std::vector<ptr> make_vector(ModifierPtr modifier)
		{
			std::vector<ptr> modifiers;
			modifiers.push_back(std::move(modifier));
			return modifiers;
		}

		/// Makes a vector of modifiers from the given non-empty variadic list of modifiers.
		template <typename FirstModifierPtr, typename... RestModifierPtrs>
		static std::vector<ptr> make_vector(FirstModifierPtr first, RestModifierPtrs... rest)
		{
			std::vector<ptr> modifiers;
			modifiers.push_back(std::move(first));
			auto rest = make_vector(std::forward<RestModifierPtrs>(rest)...);
			modifiers.insert(modifiers.end(), std::make_move_iterator(rest.begin()), std::make_move_iterator(rest.end()));
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
		constexpr MuteModifier(bool mute) : _mute{mute} {}
		static ptr make(bool mute) { return std::make_unique<MuteModifier>(mute); }
		void apply(Attributes& attributes) override { attributes.mute = _mute; }
	private:
		bool _mute;
	};

	/// Adjusts a scalar attribute by some magnitude.
	class ScalarModifier : public Modifier
	{
	public:
		constexpr ScalarModifier(double magnitude) : _magnitude{magnitude} {}
		constexpr double magnitude() const { return _magnitude; }
	private:
		double _magnitude;
	};
	struct VitalityModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Attributes& attributes) override { attributes.vitality += magnitude(); }
	};
	struct SpiritModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Attributes& attributes) override { attributes.spirit += magnitude(); }
	};
	struct HealthRegenModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Attributes& attributes) override { attributes.health_regen += magnitude(); }
	};
	struct ManaRegenModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Attributes& attributes) override { attributes.mana_regen += magnitude(); }
	};
	struct StrengthModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Attributes& attributes) override { attributes.strength += magnitude(); }
	};
	struct EnduranceModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Attributes& attributes) override { attributes.endurance += magnitude(); }
	};
	struct StaminaModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Attributes& attributes) override { attributes.stamina += magnitude(); }
	};
	struct AgilityModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Attributes& attributes) override { attributes.agility += magnitude(); }
	};
	struct DexterityModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Attributes& attributes) override { attributes.dexterity += magnitude(); }
	};
	struct StealthModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Attributes& attributes) override { attributes.stealth += magnitude(); }
	};

	// Vision Modifiers

	struct VisualAcuityModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Attributes& attributes) override { attributes.vision.acuity(attributes.vision.acuity() + magnitude()); }
	};
	struct IdealLightModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Attributes& attributes) override { attributes.vision.ideal_light(attributes.vision.ideal_light() + magnitude()); }
	};
	struct LightToleranceModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Attributes& attributes) override { attributes.vision.light_tolerance(attributes.vision.light_tolerance() + magnitude()); }
	};

	struct HearingModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Attributes& attributes) override { attributes.hearing += magnitude(); }
	};
	struct IntellectModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Attributes& attributes) override { attributes.intellect += magnitude(); }
	};
	struct LiftModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Attributes& attributes) override { attributes.lift += magnitude(); }
	};
	struct MinTempModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Attributes& attributes) override { attributes.min_temp += magnitude(); }
	};
	struct MaxTempModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Attributes& attributes) override { attributes.max_temp += magnitude(); }
	};

	// Magic Power Modifers

	struct WhitePowerModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Attributes& attributes) override { attributes.magic_power.white(attributes.magic_power.white() + magnitude()); }
	};
	struct BlackPowerModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Attributes& attributes) override { attributes.magic_power.black(attributes.magic_power.black() + magnitude()); }
	};
	struct GreenPowerModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Attributes& attributes) override { attributes.magic_power.green(attributes.magic_power.green() + magnitude()); }
	};
	struct RedPowerModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Attributes& attributes) override { attributes.magic_power.red(attributes.magic_power.red() + magnitude()); }
	};
	struct BluePowerModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Attributes& attributes) override { attributes.magic_power.blue(attributes.magic_power.blue() + magnitude()); }
	};
	struct YellowPowerModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Attributes& attributes) override { attributes.magic_power.yellow(attributes.magic_power.yellow() + magnitude()); }
	};

	// Magic Resistance Modifiers

	struct WhiteResistanceModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Attributes& attributes) override { attributes.magic_resistance.white(attributes.magic_resistance.white() + magnitude()); }
	};
	struct BlackResistanceModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Attributes& attributes) override { attributes.magic_resistance.black(attributes.magic_resistance.black() + magnitude()); }
	};
	struct GreenResistanceModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Attributes& attributes) override { attributes.magic_resistance.green(attributes.magic_resistance.green() + magnitude()); }
	};
	struct RedResistanceModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Attributes& attributes) override { attributes.magic_resistance.red(attributes.magic_resistance.red() + magnitude()); }
	};
	struct BlueResistanceModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Attributes& attributes) override { attributes.magic_resistance.blue(attributes.magic_resistance.blue() + magnitude()); }
	};
	struct YellowResistanceModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Attributes& attributes) override { attributes.magic_resistance.yellow(attributes.magic_resistance.yellow() + magnitude()); }
	};
}

#endif
