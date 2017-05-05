//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <memory>
#include <vector>

#include "Stats.h"

namespace questless
{
	//! A modification to a stat of a being.
	struct Modifier
	{
		//! @todo make_vector probably belongs in utility.h (as make_unique_vector<T> or something).

		//! Makes a vector of modifiers from the given modifier.
		template <typename ModifierPtr>
		static auto make_vector(ModifierPtr modifier)
		{
			std::vector<uptr<Modifier>> modifiers;
			modifiers.push_back(std::move(modifier));
			return modifiers;
		}

		//! Makes a vector of modifiers from the given non-empty variadic list of modifiers.
		template <typename FirstModifierPtr, typename... RestModifierPtrs>
		static auto make_vector(FirstModifierPtr first, RestModifierPtrs... rest)
		{
			std::vector<uptr<Modifier>> modifiers;
			modifiers.push_back(std::move(first));
			auto rest = make_vector(std::forward<RestModifierPtrs>(rest)...);
			modifiers.insert(modifiers.end(), std::make_move_iterator(rest.begin()), std::make_move_iterator(rest.end()));
			return modifiers;
		}

		//! Modifies the stat according to the given modifiers.
		//! @param modifiers A vector of stat modifiers.
		static void apply_all(std::vector<uptr<Modifier>> const& modifiers, Stats& stats)
		{
			for (auto const& modifier : modifiers) {
				modifier->apply(stats);
			}
		}

		virtual void apply(Stats& stats) = 0;
	};

	//! Overrides whether the being is mute.
	class MuteModifier : public Modifier
	{
	public:
		constexpr MuteModifier(bool mute) : _mute{mute} {}
		static auto make(bool mute) { return std::make_unique<MuteModifier>(mute); }
		void apply(Stats& stats) final { stats.mute = _mute; }
	private:
		bool _mute;
	};

	//! Adjusts a scalar stat by some magnitude.
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
		void apply(Stats& stats) final { stats.vitality += magnitude(); }
	};
	struct SpiritModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Stats& stats) final { stats.spirit += magnitude(); }
	};
	struct HealthRegenModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Stats& stats) final { stats.health_regen += magnitude(); }
	};
	struct ManaRegenModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Stats& stats) final { stats.mana_regen += magnitude(); }
	};
	struct StrengthModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Stats& stats) final { stats.strength += magnitude(); }
	};
	struct EnduranceModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Stats& stats) final { stats.endurance += magnitude(); }
	};
	struct StaminaModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Stats& stats) final { stats.stamina += magnitude(); }
	};
	struct AgilityModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Stats& stats) final { stats.agility += magnitude(); }
	};
	struct DexterityModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Stats& stats) final { stats.dexterity += magnitude(); }
	};
	struct StealthModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Stats& stats) final { stats.stealth += magnitude(); }
	};

	// Vision Modifiers

	struct VisualAcuityModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Stats& stats) final { stats.vision.acuity += magnitude(); }
	};
	struct IdealLuminanceModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Stats& stats) final { stats.vision.ideal_illuminance += magnitude(); }
	};
	struct LightToleranceModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Stats& stats) final { stats.vision.darkness_tolerance += magnitude(); }
	};

	struct HearingModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Stats& stats) final { stats.hearing += magnitude(); }
	};
	struct IntellectModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Stats& stats) final { stats.intellect += magnitude(); }
	};
	struct WeightModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Stats& stats) final { stats.weight += magnitude(); }
	};
	struct MinTempModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Stats& stats) final { stats.min_temp += magnitude(); }
	};
	struct MaxTempModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Stats& stats) final { stats.max_temp += magnitude(); }
	};

	// Magic Modifers

	struct WhiteMagicModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Stats& stats) final { stats.magic.white += magnitude(); }
	};
	struct BlackMagicModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Stats& stats) final { stats.magic.black += magnitude(); }
	};
	struct GreenMagicModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Stats& stats) final { stats.magic.green += magnitude(); }
	};
	struct RedMagicModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Stats& stats) final { stats.magic.red += magnitude(); }
	};
	struct BlueMagicModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Stats& stats) final { stats.magic.blue += magnitude(); }
	};
	struct YellowMagicModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Stats& stats) final { stats.magic.yellow += magnitude(); }
	};

	// Antimagic Modifiers

	struct WhiteAntimagicModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Stats& stats) final { stats.antimagic.white += magnitude(); }
	};
	struct BlackAntimagicModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Stats& stats) final { stats.antimagic.black += magnitude(); }
	};
	struct GreenAntimagicModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Stats& stats) final { stats.antimagic.green += magnitude(); }
	};
	struct RedAntimagicModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Stats& stats) final { stats.antimagic.red += magnitude(); }
	};
	struct BlueAntimagicModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Stats& stats) final { stats.antimagic.blue += magnitude(); }
	};
	struct YellowAntimagicModifier : public ScalarModifier
	{
		using ScalarModifier::ScalarModifier;
		void apply(Stats& stats) final { stats.antimagic.yellow += magnitude(); }
	};
}
