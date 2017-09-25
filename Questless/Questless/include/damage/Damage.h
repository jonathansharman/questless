//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <variant>

#include "utility/Bounded.h"

namespace questless::dmg
{
	namespace detail
	{
		//! @todo Replace with C++17 inline variable when supported.
		struct Minimum { static constexpr double value = 0.0; };

		struct Type : public Bounded<double, Minimum::value>
		{
			explicit constexpr Type(double amount) : Bounded(amount) {}
		};

		struct FactorType : public Bounded<double, Minimum::value>
		{
			explicit constexpr FactorType(double amount) : Bounded(amount) {}
		};
	}

	struct Slash : detail::Type { using detail::Type::Type; };
	struct Pierce : detail::Type { using detail::Type::Type; };
	struct Cleave : detail::Type { using detail::Type::Type; };
	struct Bludgeon : detail::Type { using detail::Type::Type; };
	struct Burn : detail::Type { using detail::Type::Type; };
	struct Freeze : detail::Type { using detail::Type::Type; };
	struct Blight : detail::Type { using detail::Type::Type; };
	struct Poison : detail::Type { using detail::Type::Type; };
	struct Shock : detail::Type { using detail::Type::Type; };

	struct SlashFactor : detail::FactorType { using detail::FactorType::FactorType; };
	struct PierceFactor : detail::FactorType { using detail::FactorType::FactorType; };
	struct CleaveFactor : detail::FactorType { using detail::FactorType::FactorType; };
	struct BludgeonFactor : detail::FactorType { using detail::FactorType::FactorType; };
	struct BurnFactor : detail::FactorType { using detail::FactorType::FactorType; };
	struct FreezeFactor : detail::FactorType { using detail::FactorType::FactorType; };
	struct BlightFactor : detail::FactorType { using detail::FactorType::FactorType; };
	struct PoisonFactor : detail::FactorType { using detail::FactorType::FactorType; };
	struct ShockFactor : detail::FactorType { using detail::FactorType::FactorType; };

	using Damage = std::variant
		< Slash
		, Pierce
		, Cleave
		, Bludgeon
		, Burn
		, Freeze
		, Blight
		, Poison
		, Shock
		>;

	using Factor = std::variant
		< SlashFactor
		, PierceFactor
		, CleaveFactor
		, BludgeonFactor
		, BurnFactor
		, FreezeFactor
		, BlightFactor
		, PoisonFactor
		, ShockFactor
		>;
}