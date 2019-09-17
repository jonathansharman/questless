//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "cancel/quantity.hpp"

#include <variant>

namespace ql {
	namespace dmg {
		// Damage

		using slash = cancel::quantity<int, cancel::unit_t<struct slash_tag>>;
		using pierce = cancel::quantity<int, cancel::unit_t<struct pierce_tag>>;
		using cleave = cancel::quantity<int, cancel::unit_t<struct cleave_tag>>;
		using bludgeon = cancel::quantity<int, cancel::unit_t<struct bludgeon_tag>>;
		using scorch = cancel::quantity<int, cancel::unit_t<struct scorch_tag>>;
		using freeze = cancel::quantity<int, cancel::unit_t<struct freeze_tag>>;
		using shock = cancel::quantity<int, cancel::unit_t<struct shock_tag>>;
		using poison = cancel::quantity<int, cancel::unit_t<struct poison_tag>>;
		using rot = cancel::quantity<int, cancel::unit_t<struct rot_tag>>;

		using damage = std::variant<slash, pierce, cleave, bludgeon, scorch, freeze, shock, poison, rot>;

		// Damage factors

		using slash_factor = cancel::quantity<int, cancel::unit_t<struct slash_factor_tag>>;
		using pierce_factor = cancel::quantity<int, cancel::unit_t<struct pierce_factor_tag>>;
		using cleave_factor = cancel::quantity<int, cancel::unit_t<struct cleave_factor_tag>>;
		using bludgeon_factor = cancel::quantity<int, cancel::unit_t<struct bludgeon_factor_tag>>;
		using scorch_factor = cancel::quantity<int, cancel::unit_t<struct scorch_factor_tag>>;
		using freeze_factor = cancel::quantity<int, cancel::unit_t<struct freeze_factor_tag>>;
		using shock_factor = cancel::quantity<int, cancel::unit_t<struct shock_factor_tag>>;
		using poison_factor = cancel::quantity<int, cancel::unit_t<struct poison_factor_tag>>;
		using rot_factor = cancel::quantity<int, cancel::unit_t<struct rot_factor_tag>>;

		using factor =
			std::variant<slash_factor, pierce_factor, cleave_factor, bludgeon_factor, scorch_factor, freeze_factor, shock_factor, poison_factor, rot_factor>;
	}

	// Damage literals

	constexpr auto operator"" _slash(unsigned long long value) {
		return dmg::slash{static_cast<int>(value)};
	}
	constexpr auto operator"" _pierce(unsigned long long value) {
		return dmg::pierce{static_cast<int>(value)};
	}
	constexpr auto operator"" _cleave(unsigned long long value) {
		return dmg::cleave{static_cast<int>(value)};
	}
	constexpr auto operator"" _bludgeon(unsigned long long value) {
		return dmg::bludgeon{static_cast<int>(value)};
	}
	constexpr auto operator"" _scorch(unsigned long long value) {
		return dmg::scorch{static_cast<int>(value)};
	}
	constexpr auto operator"" _freeze(unsigned long long value) {
		return dmg::freeze{static_cast<int>(value)};
	}
	constexpr auto operator"" _shock(unsigned long long value) {
		return dmg::shock{static_cast<int>(value)};
	}
	constexpr auto operator"" _poison(unsigned long long value) {
		return dmg::poison{static_cast<int>(value)};
	}
	constexpr auto operator"" _rot(unsigned long long value) {
		return dmg::rot{static_cast<int>(value)};
	}

	// Damage factor literals

	constexpr auto operator"" _slash_factor(unsigned long long value) {
		return dmg::slash_factor{static_cast<int>(value)};
	}
	constexpr auto operator"" _pierce_factor(unsigned long long value) {
		return dmg::pierce_factor{static_cast<int>(value)};
	}
	constexpr auto operator"" _cleave_factor(unsigned long long value) {
		return dmg::cleave_factor{static_cast<int>(value)};
	}
	constexpr auto operator"" _bludgeon_factor(unsigned long long value) {
		return dmg::bludgeon_factor{static_cast<int>(value)};
	}
	constexpr auto operator"" _scorch_factor(unsigned long long value) {
		return dmg::scorch_factor{static_cast<int>(value)};
	}
	constexpr auto operator"" _freeze_factor(unsigned long long value) {
		return dmg::freeze_factor{static_cast<int>(value)};
	}
	constexpr auto operator"" _shock_factor(unsigned long long value) {
		return dmg::shock_factor{static_cast<int>(value)};
	}
	constexpr auto operator"" _poison_factor(unsigned long long value) {
		return dmg::poison_factor{static_cast<int>(value)};
	}
	constexpr auto operator"" _rot_factor(unsigned long long value) {
		return dmg::rot_factor{static_cast<int>(value)};
	}
}
