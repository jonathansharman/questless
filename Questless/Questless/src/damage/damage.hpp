//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "cancel/quantity.hpp"

#include <variant>

namespace ql {
	namespace dmg {
		// Damage

		using slash = cancel::quantity<double, cancel::unit_t<struct slash_tag>>;
		using pierce = cancel::quantity<double, cancel::unit_t<struct pierce_tag>>;
		using cleave = cancel::quantity<double, cancel::unit_t<struct cleave_tag>>;
		using bludgeon = cancel::quantity<double, cancel::unit_t<struct bludgeon_tag>>;
		using burn = cancel::quantity<double, cancel::unit_t<struct burn_tag>>;
		using freeze = cancel::quantity<double, cancel::unit_t<struct freeze_tag>>;
		using blight = cancel::quantity<double, cancel::unit_t<struct blight_tag>>;
		using poison = cancel::quantity<double, cancel::unit_t<struct poison_tag>>;
		using shock = cancel::quantity<double, cancel::unit_t<struct shock_tag>>;

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

		// Damage factors

		using slash_factor = cancel::quantity<double, cancel::unit_t<struct slash_factor_tag>>;
		using pierce_factor = cancel::quantity<double, cancel::unit_t<struct pierce_factor_tag>>;
		using cleave_factor = cancel::quantity<double, cancel::unit_t<struct cleave_factor_tag>>;
		using bludgeon_factor = cancel::quantity<double, cancel::unit_t<struct bludgeon_factor_tag>>;
		using burn_factor = cancel::quantity<double, cancel::unit_t<struct burn_factor_tag>>;
		using freeze_factor = cancel::quantity<double, cancel::unit_t<struct freeze_factor_tag>>;
		using blight_factor = cancel::quantity<double, cancel::unit_t<struct blight_factor_tag>>;
		using poison_factor = cancel::quantity<double, cancel::unit_t<struct poison_factor_tag>>;
		using shock_factor = cancel::quantity<double, cancel::unit_t<struct shock_factor_tag>>;

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
	
	// Damage literals

	constexpr dmg::slash operator "" _slash(long double value) { return dmg::slash{static_cast<double>(value)}; }
	constexpr dmg::pierce operator "" _pierce(long double value) { return dmg::pierce{static_cast<double>(value)}; }
	constexpr dmg::cleave operator "" _cleave(long double value) { return dmg::cleave{static_cast<double>(value)}; }
	constexpr dmg::bludgeon operator "" _bludgeon(long double value) { return dmg::bludgeon{static_cast<double>(value)}; }
	constexpr dmg::burn operator "" _burn(long double value) { return dmg::burn{static_cast<double>(value)}; }
	constexpr dmg::freeze operator "" _freeze(long double value) { return dmg::freeze{static_cast<double>(value)}; }
	constexpr dmg::blight operator "" _blight(long double value) { return dmg::blight{static_cast<double>(value)}; }
	constexpr dmg::poison operator "" _poison(long double value) { return dmg::poison{static_cast<double>(value)}; }
	constexpr dmg::shock operator "" _shock(long double value) { return dmg::shock{static_cast<double>(value)}; }

	// Damage factor literals

	constexpr dmg::slash_factor operator "" _slash_factor(long double value) { return dmg::slash_factor{static_cast<double>(value)}; }
	constexpr dmg::pierce_factor operator "" _pierce_factor(long double value) { return dmg::pierce_factor{static_cast<double>(value)}; }
	constexpr dmg::cleave_factor operator "" _cleave_factor(long double value) { return dmg::cleave_factor{static_cast<double>(value)}; }
	constexpr dmg::bludgeon_factor operator "" _bludgeon_factor(long double value) { return dmg::bludgeon_factor{static_cast<double>(value)}; }
	constexpr dmg::burn_factor operator "" _burn_factor(long double value) { return dmg::burn_factor{static_cast<double>(value)}; }
	constexpr dmg::freeze_factor operator "" _freeze_factor(long double value) { return dmg::freeze_factor{static_cast<double>(value)}; }
	constexpr dmg::blight_factor operator "" _blight_factor(long double value) { return dmg::blight_factor{static_cast<double>(value)}; }
	constexpr dmg::poison_factor operator "" _poison_factor(long double value) { return dmg::poison_factor{static_cast<double>(value)}; }
	constexpr dmg::shock_factor operator "" _shock_factor(long double value) { return dmg::shock_factor{static_cast<double>(value)}; }
}