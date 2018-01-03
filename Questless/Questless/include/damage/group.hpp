//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <cmath>
#include <variant>
#include <vector>

#include "damage.hpp"
#include "multiplier.hpp"
#include "protect.hpp"

namespace ql::dmg
{
	//! Represents a single group of damage to a being, possibly including multiple types and quantities of damage and protection evasion.
	class group
	{
	public:
		static constexpr double pct_pad_to_slash = 0.50;
		static constexpr double pct_pad_to_pierce = 0.25;
		static constexpr double pct_pad_to_cleave = 0.75;
		static constexpr double pct_pad_to_bludgeon = 1.00;

		static constexpr double pct_deflect_to_slash = 1.00;
		static constexpr double pct_deflect_to_pierce = 0.50;
		static constexpr double pct_deflect_to_cleave = 0.75;
		static constexpr double pct_deflect_to_bludgeon = 0.25;

		static constexpr double pct_fireproof_to_burn = 1.00;
		static constexpr double pct_frostproof_to_freeze = 1.00;

		static constexpr double pct_fortify_to_blight = 1.00;

		static constexpr double pct_immunize_to_poison = 1.00;

		static constexpr double pct_insulate_to_shock = 1.00;

		group() = default;

		//! Constructs a damage group from a list of damage components.
		//! @param parts The list of components in this damage group.
		//! @param protection_evasion The amount of protection this damage group evades.
		group(std::vector<damage> parts, protect protection_evasion = protect::zero())
			: _parts{std::move(parts)}
			, _protection_evasion{protection_evasion}
		{}

		//! Constructs a damage group from a single damage component.
		//! @param part The single component of this damage group.
		//! @param protection_evasion The amount of protection this damage group evades.
		template <typename Damage>
		group(Damage damage_part, protect protection_evasion = protect::zero())
			: group{damage{damage_part}, protection_evasion}
		{}

		static group zero() { return group{}; }

		friend group operator +(group const& d1, group const& d2)
		{
			group sum = d1;
			sum += d2;
			return sum;
		}
		friend group operator *(group const& d, double k)
		{
			group product = d;
			product *= k;
			return product;
		}
		friend group operator *(double k, group const& d)
		{
			group product = d;
			product *= k;
			return product;
		}
		friend group operator /(group const& d, double k)
		{
			group quotient = d;
			quotient /= k;
			return quotient;
		}

		group& operator +=(group const& d)
		{
			_parts.insert(_parts.end(), d._parts.begin(), d._parts.end());
			_protection_evasion += d._protection_evasion;
			return *this;
		}
		group& operator *=(double k)
		{
			for (auto& part : _parts) {
				std::visit([k](auto&& part) { std::forward<decltype(part)>(part) *= k; }, part);
			}
			_protection_evasion *= k;
			return *this;
		}
		group& operator /=(double k)
		{
			for (auto& part : _parts) {
				std::visit([k](auto&& part) { std::forward<decltype(part)>(part) /= k; }, part);
			}
			_protection_evasion /= k;
			return *this;
		}

		//! The different parts/components of the damage group.
		std::vector<damage> const& parts() const { return _parts; }

		//! The amount of protection this damage group is capable of bypassing.
		protect const& protection_evasion() const { return _protection_evasion; }

		//! The sum of the damage components.
		double total() const
		{
			double result = 0;
			for (auto const& part : _parts) {
				std::visit([&result](auto&& part) { result += std::forward<decltype(part)>(part); }, part);
			}
			return result;
		}

		//! Damage group adjusted by the given @protection, @resistance, and @vulnerability.
		group with(protect const& protection, resist const& resistance, vuln const& vulnerability) const;
	private:
		std::vector<damage> _parts;
		protect _protection_evasion;

		//! Helper @param protection_evasion The amount of protection this damage group evades.
		group(damage damage_part, protect protection_evasion = protect::zero())
			: _parts{{damage_part}}
			, _protection_evasion{protection_evasion}
		{}
	};
}
