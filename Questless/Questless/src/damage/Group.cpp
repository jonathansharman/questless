//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "damage/Group.h"

namespace questless::dmg
{
	Group Group::with(Protect const& protection) const
	{
		Group result = *this;

		// Reduce protection by the damage's protection evasion.
		Protect const reduced_protection = protection - result._protection_evasion;

		// Reduce the result's protection evasion by the amount spent on this protection.
		result._protection_evasion -= protection - reduced_protection;

		// Reduce damage by the reduced protection.
		for (auto& part : result._parts) {
			struct ProtectionApplier
			{
				Protect const& reduced_protection;

				void operator ()(Slash& slash) const
				{
					slash -= reduced_protection.pad() * percent_pad_to_slash + reduced_protection.deflect() * percent_deflect_to_slash;
				}
				void operator ()(Pierce& pierce) const
				{
					pierce -= reduced_protection.pad() * percent_pad_to_pierce + reduced_protection.deflect() * percent_deflect_to_pierce;
				}
				void operator ()(Cleave& cleave) const
				{
					cleave -= reduced_protection.pad() * percent_pad_to_cleave + reduced_protection.deflect() * percent_deflect_to_cleave;
				}
				void operator ()(Bludgeon& bludgeon) const
				{
					bludgeon -= reduced_protection.pad() * percent_pad_to_bludgeon + reduced_protection.deflect() * percent_deflect_to_bludgeon;
				}
				void operator ()(Burn& burn) const
				{
					burn -= reduced_protection.fireproof() * percent_fireproof_to_burn;
				}
				void operator ()(Freeze& freeze) const
				{
					freeze -= reduced_protection.frostproof() * percent_frostproof_to_freeze;
				}
				void operator ()(Blight& blight) const
				{
					blight -= reduced_protection.fortify() * percent_fortify_to_blight;
				}
				void operator ()(Poison& poison) const
				{
					poison -= reduced_protection.immunize() * percent_immunize_to_poison;
				}
				void operator ()(Shock& shock) const
				{
					shock -= reduced_protection.insulate() * percent_insulate_to_shock;
				}
			};
			std::visit(ProtectionApplier{reduced_protection}, part);
		}
		return result;
	}

	Group Group::with(Resist const& resistance, Vuln const& vulnerability) const
	{
		Group result = *this;
		for (auto& part : result._parts) {
			struct DamageMultiplierApplier
			{
				Resist const& resistance;
				Vuln const& vulnerability;

				void operator ()(Slash& slash) const
				{
					slash = Slash{std::max(0.0, slash * (1.0 + (vulnerability.slash() - resistance.slash()) / 100.0))};
				}
				void operator ()(Pierce& pierce) const
				{
					pierce = Pierce{std::max(0.0, pierce * (1.0 + (vulnerability.pierce() - resistance.pierce()) / 100.0))};
				}
				void operator ()(Cleave& cleave) const
				{
					cleave = Cleave{std::max(0.0, cleave * (1.0 + (vulnerability.cleave() - resistance.cleave()) / 100.0))};
				}
				void operator ()(Bludgeon& bludgeon) const
				{
					bludgeon = Bludgeon{std::max(0.0, bludgeon * (1.0 + (vulnerability.bludgeon() - resistance.bludgeon()) / 100.0))};
				}
				void operator ()(Burn& burn) const
				{
					burn = Burn{std::max(0.0, burn * (1.0 + (vulnerability.burn() - resistance.burn()) / 100.0))};
				}
				void operator ()(Freeze& freeze) const
				{
					freeze = Freeze{std::max(0.0, freeze * (1.0 + (vulnerability.freeze() - resistance.freeze()) / 100.0))};
				}
				void operator ()(Blight& blight) const
				{
					blight = Blight{std::max(0.0, blight * (1.0 + (vulnerability.blight() - resistance.blight()) / 100.0))};
				}
				void operator ()(Poison& poison) const
				{
					poison = Poison{std::max(0.0, poison * (1.0 + (vulnerability.poison() - resistance.poison()) / 100.0))};
				}
				void operator ()(Shock& shock) const
				{
					shock = Shock{std::max(0.0, shock * (1.0 + (vulnerability.shock() - resistance.shock()) / 100.0))};
				}
			};
			std::visit(DamageMultiplierApplier{resistance, vulnerability}, part);
		}
		return result;
	}
}
