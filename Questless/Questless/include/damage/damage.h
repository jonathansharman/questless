//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <variant>

#include "utility/static_bounded.h"

namespace ql::dmg
{
	namespace detail
	{
		//! @todo Replace with C++17 inline variable when supported.
		struct minimum { static constexpr double value = 0.0; };

		struct type : public static_bounded<double, minimum::value>
		{
			explicit constexpr type(double amount) : static_bounded(amount) {}
		};

		struct factor_type : public static_bounded<double, minimum::value>
		{
			explicit constexpr factor_type(double amount) : static_bounded(amount) {}
		};
	}

	struct slash : detail::type { using detail::type::type; };
	struct pierce : detail::type { using detail::type::type; };
	struct cleave : detail::type { using detail::type::type; };
	struct bludgeon : detail::type { using detail::type::type; };
	struct burn : detail::type { using detail::type::type; };
	struct freeze : detail::type { using detail::type::type; };
	struct blight : detail::type { using detail::type::type; };
	struct poison : detail::type { using detail::type::type; };
	struct shock : detail::type { using detail::type::type; };

	struct slash_factor : detail::factor_type { using detail::factor_type::factor_type; };
	struct pierce_factor : detail::factor_type { using detail::factor_type::factor_type; };
	struct cleave_factor : detail::factor_type { using detail::factor_type::factor_type; };
	struct bludgeon_factor : detail::factor_type { using detail::factor_type::factor_type; };
	struct burn_factor : detail::factor_type { using detail::factor_type::factor_type; };
	struct freeze_factor : detail::factor_type { using detail::factor_type::factor_type; };
	struct blight_factor : detail::factor_type { using detail::factor_type::factor_type; };
	struct poison_factor : detail::factor_type { using detail::factor_type::factor_type; };
	struct shock_factor : detail::factor_type { using detail::factor_type::factor_type; };

	using damage = std::variant
		< slash
		, pierce
		, cleave
		, bludgeon
		, burn
		, freeze
		, blight
		, poison
		, shock
		>;

	using factor = std::variant
		< slash_factor
		, pierce_factor
		, cleave_factor
		, bludgeon_factor
		, burn_factor
		, freeze_factor
		, blight_factor
		, poison_factor
		, shock_factor
		>;
}