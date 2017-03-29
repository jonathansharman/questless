/**
* @file    Damage.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*/

#pragma once

#include <vector>
#include <cmath>

#include "DamageMultiplier.h"
#include "Protection.h"
#include "utility/Bounded.h"

namespace questless
{
	/// Represents a single instance of damage to a being, possibly including multiple types and quantities of damage and protection evasion.
	////
	class Damage
	{
	public:
		struct Part
		{
			enum class Type { slash, pierce, cleave, bludgeon, burn, freeze, blight };

			static constexpr double minimum_value = 0.0;

			Type type;
			Bounded<double, minimum_value> amount;
		};

		static constexpr double percent_pad_to_slash = 0.50;
		static constexpr double percent_pad_to_pierce = 0.25;
		static constexpr double percent_pad_to_cleave = 0.75;
		static constexpr double percent_pad_to_bludgeon = 1.00;

		static constexpr double percent_deflect_to_slash = 1.00;
		static constexpr double percent_deflect_to_pierce = 0.75;
		static constexpr double percent_deflect_to_cleave = 0.50;
		static constexpr double percent_deflect_to_bludgeon = 0.25;

		static constexpr double percent_fireproof_to_burn = 1.00;
		static constexpr double percent_frostproof_to_freeze = 1.00;

		static constexpr double percent_cleanse_to_blight = 1.00;

		Damage() = default;

		Damage(std::vector<Part> parts, Protection protection_evasion = Protection::zero())
			: _parts{std::move(parts)}
			, _protection_evasion{protection_evasion}
		{}

		Damage(Slash slash, Protection protection_evasion = Protection::zero())
			: _parts{{Part{Part::Type::slash, slash}}}
			, _protection_evasion{protection_evasion}
		{}
		Damage(Pierce pierce, Protection protection_evasion = Protection::zero())
			: _parts{{Part{Part::Type::pierce, pierce}}}
			, _protection_evasion{protection_evasion}
		{}
		Damage(Cleave cleave, Protection protection_evasion = Protection::zero())
			: _parts{{Part{Part::Type::cleave, cleave}}}
			, _protection_evasion{protection_evasion}
		{}
		Damage(Bludgeon bludgeon, Protection protection_evasion = Protection::zero())
			: _parts{{Part{Part::Type::bludgeon, bludgeon}}}
			, _protection_evasion{protection_evasion}
		{}
		Damage(Burn burn, Protection protection_evasion = Protection::zero())
			: _parts{{Part{Part::Type::burn, burn}}}
			, _protection_evasion{protection_evasion}
		{}
		Damage(Freeze freeze, Protection protection_evasion = Protection::zero())
			: _parts{{Part{Part::Type::freeze, freeze}}}
			, _protection_evasion{protection_evasion}
		{}
		Damage(Blight blight, Protection protection_evasion = Protection::zero())
			: _parts{{Part{Part::Type::blight, blight}}}
			, _protection_evasion{protection_evasion}
		{}

		static Damage zero() { return Damage{}; }

		friend Damage operator +(Damage const& d1, Damage const& d2)
		{
			Damage sum = d1;
			sum += d2;
			return sum;
		}
		friend Damage operator *(Damage const& d, double k)
		{
			Damage product = d;
			product *= k;
			return product;
		}
		friend Damage operator *(double k, Damage const& d)
		{
			Damage product = d;
			product *= k;
			return product;
		}
		friend Damage operator /(Damage const& d, double k)
		{
			Damage quotient = d;
			for (auto& part : quotient._parts) {
				part.amount /= k;
			}
			return quotient;
		}

		Damage& operator +=(Damage const& d)
		{
			_parts.insert(_parts.end(), d._parts.begin(), d._parts.end());
			return *this;
		}
		Damage& operator *=(double k)
		{
			for (Part& part : _parts) {
				part.amount *= k;
			}
			return *this;
		}

		/// @return The different parts/components of the damage instance.
		////
		std::vector<Part> const& parts() const { return _parts; }

		/// @return The amount of protection this damage instance is capable of bypassing.
		////
		Protection const& protection_evasion() const { return _protection_evasion; }

		/// @return The sum of the damage components.
		////
		double total() const
		{
			double result = 0;
			for (Part const& part : _parts) {
				result += part.amount;
			}
			return result;
		}

		/// @return Damage adjusted by the given protection.
		////
		Damage with(Protection const& protection) const;

		/// @return Damage adjusted by the given resistance and vulnerability.
		////
		Damage with(Resistance const& resistance, Vulnerability const& vulnerability) const;
	private:
		std::vector<Part> _parts;
		Protection _protection_evasion;
	};
}
