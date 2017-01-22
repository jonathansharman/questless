/**
* @file    Modifier.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Represents a modification to a stat.
*/

#ifndef MODIFIER_H
#define MODIFIER_H

#include <memory>
#include <vector>

#include "Stats.h"

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

		/// Modifies the stat according to the given modifiers.
		/// @param modifiers A vector of stat modifiers.
		static void apply_all(const std::vector<Modifier::ptr>& modifiers, Stats& stats)
		{
			for (const auto& modifier : modifiers) {
				modifier->apply(stats);
			}
		}

		virtual void apply(Stats& stats) = 0;
	};

	/// Overrides whether the being is mute.
	class MuteModifier : public Modifier
	{
	public:
		constexpr MuteModifier(bool mute) : _mute{mute} {}
		static ptr make(bool mute) { return std::make_unique<MuteModifier>(mute); }
		void apply(Stats& stats) override { stats.mute = _mute; }
	private:
		bool _mute;
	};

	/// Adjusts a scalar stat by some magnitude.
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
		void apply(Stats& stats) override { stats.vitality += magnitude(); }
	};
	struct SpiritModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Stats& stats) override { stats.spirit += magnitude(); }
	};
	struct HealthRegenModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Stats& stats) override { stats.health_regen += magnitude(); }
	};
	struct ManaRegenModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Stats& stats) override { stats.mana_regen += magnitude(); }
	};
	struct StrengthModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Stats& stats) override { stats.strength += magnitude(); }
	};
	struct EnduranceModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Stats& stats) override { stats.endurance += magnitude(); }
	};
	struct StaminaModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Stats& stats) override { stats.stamina += magnitude(); }
	};
	struct AgilityModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Stats& stats) override { stats.agility += magnitude(); }
	};
	struct DexterityModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Stats& stats) override { stats.dexterity += magnitude(); }
	};
	struct StealthModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Stats& stats) override { stats.stealth += magnitude(); }
	};

	// Vision Modifiers

	struct VisualAcuityModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Stats& stats) override { stats.vision.acuity(stats.vision.acuity() + magnitude()); }
	};
	struct IdealLightModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Stats& stats) override { stats.vision.ideal_light(stats.vision.ideal_light() + magnitude()); }
	};
	struct LightToleranceModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Stats& stats) override { stats.vision.light_tolerance(stats.vision.light_tolerance() + magnitude()); }
	};

	struct HearingModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Stats& stats) override { stats.hearing += magnitude(); }
	};
	struct IntellectModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Stats& stats) override { stats.intellect += magnitude(); }
	};
	struct WeightModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Stats& stats) override { stats.weight += magnitude(); }
	};
	struct MinTempModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Stats& stats) override { stats.min_temp += magnitude(); }
	};
	struct MaxTempModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Stats& stats) override { stats.max_temp += magnitude(); }
	};

	// Magic Modifers

	struct WhiteMagicModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Stats& stats) override { stats.magic.white += magnitude(); }
	};
	struct BlackMagicModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Stats& stats) override { stats.magic.black += magnitude(); }
	};
	struct GreenMagicModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Stats& stats) override { stats.magic.green += magnitude(); }
	};
	struct RedMagicModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Stats& stats) override { stats.magic.red += magnitude(); }
	};
	struct BlueMagicModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Stats& stats) override { stats.magic.blue += magnitude(); }
	};
	struct YellowMagicModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Stats& stats) override { stats.magic.yellow += magnitude(); }
	};

	// Antimagic Modifiers

	struct WhiteAntimagicModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Stats& stats) override { stats.antimagic.white += magnitude(); }
	};
	struct BlackAntimagicModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Stats& stats) override { stats.antimagic.black += magnitude(); }
	};
	struct GreenAntimagicModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Stats& stats) override { stats.antimagic.green += magnitude(); }
	};
	struct RedAntimagicModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Stats& stats) override { stats.antimagic.red += magnitude(); }
	};
	struct BlueAntimagicModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Stats& stats) override { stats.antimagic.blue += magnitude(); }
	};
	struct YellowAntimagicModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Stats& stats) override { stats.antimagic.yellow += magnitude(); }
	};
}

#endif
