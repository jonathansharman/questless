/**
* @file    Damage.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The Damage type, which encapsulates a set of damage components inflicted together.
*/

#pragma once

#include <vector>
#include <cmath>

#include "DamageMultiplier.h"
#include "Protection.h"
#include "utility/Bounded.h"

namespace questless
{
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

		Damage() = default;

		Damage(std::vector<Part> parts) : _parts{std::move(parts)} {}

		Damage(Slash slash) : _parts{{Part{Part::Type::slash, slash}}} {}
		Damage(Pierce pierce) : _parts{{Part{Part::Type::pierce, pierce}}} {}
		Damage(Cleave cleave) : _parts{{Part{Part::Type::cleave, cleave}}} {}
		Damage(Bludgeon bludgeon) : _parts{{Part{Part::Type::bludgeon, bludgeon}}} {}
		Damage(Burn burn) : _parts{{Part{Part::Type::burn, burn}}} {}
		Damage(Freeze freeze) : _parts{{Part{Part::Type::freeze, freeze}}} {}
		Damage(Blight blight) : _parts{{Part{Part::Type::blight, blight}}} {}

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

		/// @return Damage adjusted by the given protection.
		Damage with(Protection const& protection) const
		{
			Damage result = *this;
			for (Part& part : result._parts) {
				switch (part.type) {
					case Part::Type::slash:
						part.amount -= protection.pad() * Protection::percent_pad_to_slash + protection.deflect() * Protection::percent_deflect_to_slash;
						break;
					case Part::Type::pierce:
						part.amount -= protection.pad() * Protection::percent_pad_to_pierce + protection.deflect() * Protection::percent_deflect_to_pierce;
						break;
					case Part::Type::cleave:
						part.amount -= protection.pad() * Protection::percent_pad_to_cleave + protection.deflect() * Protection::percent_deflect_to_cleave;
						break;
					case Part::Type::bludgeon:
						part.amount -= protection.pad() * Protection::percent_pad_to_bludgeon + protection.deflect() * Protection::percent_deflect_to_bludgeon;
						break;
					case Part::Type::burn:
						part.amount -= protection.fireproof() * Protection::percent_fireproof_to_burn;
						break;
					case Part::Type::freeze:
						part.amount -= protection.frostproof() * Protection::percent_frostproof_to_freeze;
						break;
					case Part::Type::blight:
						part.amount -= protection.cleanse() * Protection::percent_cleanse_to_blight;
						break;
				}
			}
			return result;
		}

		/// @return Damage adjusted by the given resistance and vulnerability.
		Damage with(Resistance const& resistance, Vulnerability const& vulnerability) const
		{
			Damage result = *this;
			for (Part& part : result._parts) {
				switch (part.type) {
					case Part::Type::slash:
						part.amount = std::max(0.0, part.amount * (1.0 + (vulnerability.slash() - resistance.slash()) / 100.0));
						break;
					case Part::Type::pierce:
						part.amount = std::max(0.0, part.amount * (1.0 + (vulnerability.pierce() - resistance.pierce()) / 100.0));
						break;
					case Part::Type::cleave:
						part.amount = std::max(0.0, part.amount * (1.0 + (vulnerability.cleave() - resistance.cleave()) / 100.0));
						break;
					case Part::Type::bludgeon:
						part.amount = std::max(0.0, part.amount * (1.0 + (vulnerability.bludgeon() - resistance.bludgeon()) / 100.0));
						break;
					case Part::Type::burn:
						part.amount = std::max(0.0, part.amount * (1.0 + (vulnerability.burn() - resistance.burn()) / 100.0));
						break;
					case Part::Type::freeze:
						part.amount = std::max(0.0, part.amount * (1.0 + (vulnerability.freeze() - resistance.freeze()) / 100.0));
						break;
					case Part::Type::blight:
						part.amount = std::max(0.0, part.amount * (1.0 + (vulnerability.blight() - resistance.blight()) / 100.0));
						break;
				}
			}
			return result;
		}

		/// @return The different parts/components of the damage instance.
		////
		std::vector<Part> const& parts() const { return _parts; }

		/// @return The sum of the damage components.
		double total() const
		{
			double result = 0;
			for (Part const& part : _parts) {
				result += part.amount;
			}
			return result;
		}
	private:
		std::vector<Part> _parts;
	};
}
