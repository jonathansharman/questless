//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <cmath>
#include <variant>
#include <vector>

#include "Damage.h"
#include "Multiplier.h"
#include "Protect.h"

namespace questless::dmg
{
	//! Represents a single group of damage to a being, possibly including multiple types and quantities of damage and protection evasion.
	class Group
	{
	public:
		static constexpr double percent_pad_to_slash = 0.50;
		static constexpr double percent_pad_to_pierce = 0.25;
		static constexpr double percent_pad_to_cleave = 0.75;
		static constexpr double percent_pad_to_bludgeon = 1.00;

		static constexpr double percent_deflect_to_slash = 1.00;
		static constexpr double percent_deflect_to_pierce = 0.50;
		static constexpr double percent_deflect_to_cleave = 0.75;
		static constexpr double percent_deflect_to_bludgeon = 0.25;

		static constexpr double percent_fireproof_to_burn = 1.00;
		static constexpr double percent_frostproof_to_freeze = 1.00;

		static constexpr double percent_cleanse_to_blight = 1.00;

		static constexpr double percent_immunize_to_poison = 1.00;

		Group() = default;

		//! @param parts The list of components in this damage group.
		//! @param protection_evasion The amount of protection this damage group evades.
		Group(std::vector<Damage> parts, Protect protection_evasion = Protect::zero())
			: _parts{std::move(parts)}
			, _protection_evasion{protection_evasion}
		{}

		//! Constructs a damage group from a single damage component.
		//! @param part The single component of this damage group.
		//! @param protection_evasion The amount of protection this damage group evades.
		Group(Damage damage_part, Protect protection_evasion = Protect::zero())
			: _parts{{damage_part}}
			, _protection_evasion{protection_evasion}
		{}

		static Group zero() { return Group{}; }

		friend Group operator +(Group const& d1, Group const& d2)
		{
			Group sum = d1;
			sum += d2;
			return sum;
		}
		friend Group operator *(Group const& d, double k)
		{
			Group product = d;
			product *= k;
			return product;
		}
		friend Group operator *(double k, Group const& d)
		{
			Group product = d;
			product *= k;
			return product;
		}
		friend Group operator /(Group const& d, double k)
		{
			Group quotient = d;
			quotient /= k;
			return quotient;
		}

		Group& operator +=(Group const& d)
		{
			_parts.insert(_parts.end(), d._parts.begin(), d._parts.end());
			_protection_evasion += d._protection_evasion;
			return *this;
		}
		Group& operator *=(double k)
		{
			for (auto& part : _parts) {
				std::visit([k](auto&& part) { std::forward<decltype(part)>(part) *= k; }, part);
			}
			_protection_evasion *= k;
			return *this;
		}
		Group& operator /=(double k)
		{
			for (auto& part : _parts) {
				std::visit([k](auto&& part) { std::forward<decltype(part)>(part) /= k; }, part);
			}
			_protection_evasion /= k;
			return *this;
		}

		//! The different parts/components of the damage group.
		std::vector<Damage> const& parts() const { return _parts; }

		//! The amount of protection this damage group is capable of bypassing.
		Protect const& protection_evasion() const { return _protection_evasion; }

		//! The sum of the damage components.
		double total() const
		{
			double result = 0;
			for (auto const& part : _parts) {
				std::visit([&result](auto&& part) { result += std::forward<decltype(part)>(part); }, part);
			}
			return result;
		}

		//! Damage group adjusted by the given protection.
		Group with(Protect const& protection) const;

		//! Damage group adjusted by the given resistance and vulnerability.
		Group with(Resist const& resistance, Vuln const& vulnerability) const;
	private:
		std::vector<Damage> _parts;
		Protect _protection_evasion;
	};
}
