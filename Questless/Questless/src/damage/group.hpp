//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "damage.hpp"
#include "multiplier.hpp"
#include "protect.hpp"

#include "quantities/misc.hpp"
#include "utility/static_bounded.hpp"

#include <cmath>
#include <vector>

namespace ql::dmg {
	//! Represents a single group of damage to a being, possibly including multiple types and quantities of damage and
	//! protection bypass.
	struct group {
		static constexpr auto slash_per_pad = 0.50_slash / 1.0_pad;
		static constexpr auto pierce_per_pad = 0.25_pierce / 1.0_pad;
		static constexpr auto cleave_per_pad = 0.75_cleave / 1.0_pad;
		static constexpr auto bludgeon_per_pad = 1.00_bludgeon / 1.0_pad;

		static constexpr auto slash_per_deflect = 1.00_slash / 1.0_deflect;
		static constexpr auto pierce_per_deflect = 0.50_pierce / 1.0_deflect;
		static constexpr auto cleave_per_deflect = 0.75_cleave / 1.0_deflect;
		static constexpr auto bludgeon_per_deflect = 0.25_bludgeon / 1.0_deflect;

		static constexpr auto burn_per_fireproof = 1.00_burn / 1.0_fireproof;
		static constexpr auto freeze_per_frostproof = 1.00_freeze / 1.0_frostproof;

		static constexpr auto blight_per_fortify = 1.00_blight / 1.0_fortify;

		static constexpr auto poison_per_immunize = 1.00_poison / 1.0_immunize;

		static constexpr auto shock_per_insulate = 1.00_shock / 1.0_insulate;

		group() = default;

		//! Constructs a damage group from a list of damage components.
		//! @param parts The list of components in this damage group.
		//! @param bypass The amount of coverage this group can bypass.
		group(std::vector<damage> parts, coverage bypass = 0.0_coverage) : _parts{std::move(parts)}, _bypass{bypass} {}

		//! Constructs a damage group from a single damage component.
		//! @param part The single component of this damage group.
		//! @param bypass The amount of coverage this group can bypass.
		template <typename Damage>
		group(Damage damage_part, coverage bypass = 0.0_coverage) : group{damage{damage_part}, bypass} {}

		static group zero() {
			return group{};
		}

		friend group operator*(group const& d, double k) {
			group product = d;
			product *= k;
			return product;
		}
		friend group operator*(double k, group const& d) {
			group product = d;
			product *= k;
			return product;
		}
		friend group operator/(group const& d, double k) {
			group quotient = d;
			quotient /= k;
			return quotient;
		}

		group& operator*=(double k) {
			for (auto& part : _parts) {
				std::visit([k](auto& part) { part *= k; }, part);
			}
			return *this;
		}
		group& operator/=(double k) {
			for (auto& part : _parts) {
				std::visit([k](auto& part) { part /= k; }, part);
			}
			return *this;
		}

		//! The different parts/components of the damage group.
		std::vector<damage> const& parts() const {
			return _parts;
		}

		//! The amount of coverage this group can bypass.
		coverage bypass() const {
			return _bypass;
		}

		//! The amount of health loss this damage group causes, defined as the sum of its damage component values.
		health health_loss() const {
			health result = 0.0_hp;
			for (auto const& part : _parts) {
				std::visit([&result](auto const& part) { result += health{part.value}; }, part);
			}
			return result;
		}

		//! Damage group adjusted by the given @protection, @resistance, and @vulnerability.
		group with(protect const& protection, resist const& resistance, vuln const& vulnerability) const;

	private:
		std::vector<damage> _parts;
		static_bounded<coverage, min_coverage, max_coverage> _bypass;

		//! Helper for constructing a damage group from a single damage part and optional protection bypass.
		group(damage damage_part, coverage bypass = 0.0_coverage) : _parts{{damage_part}}, _bypass{bypass} {}
	};
}
