//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "armor.hpp"

#include "bounded/static.hpp"
#include "quantities/misc.hpp"
#include "utility/visitation.hpp"

#include <cmath>
#include <vector>

namespace ql::dmg {
	//! Represents a single group of damage to a being, possibly including multiple types and quantities of damage and
	//! protection bypass.
	struct group {
		//! The different parts/components of the damage group.
		std::vector<damage> parts{};

		//! The amount of coverage this group can bypass.
		static_bounded<coverage, min_coverage, max_coverage> bypass = 0.0_coverage;

		group() = default;

		//! Constructs a damage group from a list of damage components.
		//! @param parts The list of components in this damage group.
		//! @param bypass The amount of coverage this group can bypass.
		group(std::vector<damage> parts, coverage bypass = 0.0_coverage) : parts{std::move(parts)}, bypass{bypass} {}

		//! Constructs a damage group from a single damage component.
		//! @param part The single component of this damage group.
		//! @param bypass The amount of coverage this group can bypass.
		group(damage damage, coverage bypass = 0.0_coverage) : parts{{damage}}, bypass{bypass} {}

		auto& operator*=(int k) {
			for (auto& part : parts) {
				std::visit([k](auto& part) { part *= k; }, part);
			}
			return *this;
		}
		auto& operator/=(int k) {
			for (auto& part : parts) {
				std::visit([k](auto& part) { part /= k; }, part);
			}
			return *this;
		}

		//! Damage group adjusted after going through @p armor.
		auto against(armor const& armor) const -> group;
	};

	inline auto operator*(group d, int k) {
		d *= k;
		return d;
	}

	inline auto operator*(int k, group d) {
		// Multiplication of int is commutative, so it's okay to delegate to d * k.
		return d * k;
	}

	inline auto operator/(group d, int k) {
		d /= k;
		return d;
	}
}
