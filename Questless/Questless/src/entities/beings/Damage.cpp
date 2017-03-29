/**
* @file    Damage.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*/

#include "entities/beings/Damage.h"

namespace questless
{
	Damage Damage::with(Protection const& protection) const
	{
		Damage result = *this;

		// Reduce protection by the damage's protection evasion.
		Protection const reduced_protection = protection - result._protection_evasion;

		// Reduce the result's protection evasion by the amount spent on this protection.
		result._protection_evasion -= protection - reduced_protection;

		// Reduce damage by the reduced protection.
		for (Part& part : result._parts) {
			switch (part.type) {
				case Part::Type::slash:
					part.amount -= reduced_protection.pad() * percent_pad_to_slash + reduced_protection.deflect() * percent_deflect_to_slash;
					break;
				case Part::Type::pierce:
					part.amount -= reduced_protection.pad() * percent_pad_to_pierce + reduced_protection.deflect() * percent_deflect_to_pierce;
					break;
				case Part::Type::cleave:
					part.amount -= reduced_protection.pad() * percent_pad_to_cleave + reduced_protection.deflect() * percent_deflect_to_cleave;
					break;
				case Part::Type::bludgeon:
					part.amount -= reduced_protection.pad() * percent_pad_to_bludgeon + reduced_protection.deflect() * percent_deflect_to_bludgeon;
					break;
				case Part::Type::burn:
					part.amount -= reduced_protection.fireproof() * percent_fireproof_to_burn;
					break;
				case Part::Type::freeze:
					part.amount -= reduced_protection.frostproof() * percent_frostproof_to_freeze;
					break;
				case Part::Type::blight:
					part.amount -= reduced_protection.cleanse() * percent_cleanse_to_blight;
					break;
			}
		}
		return result;
	}

	Damage Damage::with(Resistance const& resistance, Vulnerability const& vulnerability) const
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
}
