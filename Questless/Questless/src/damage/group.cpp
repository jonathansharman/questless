//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "damage/group.hpp"

namespace ql::dmg {
	group group::with(protect const& protection, resist const& resistance, vuln const& vulnerability) const {
		group result = *this;

		// Reduce protection by the damage's protection bypass.
		protect const reduced_protection = protection - result._protection_bypass;

		// Reduce the result's protection bypass by the amount spent on this protection.
		result._protection_bypass -= protection - reduced_protection;

		// Reduce damage by the reduced protection.
		for (auto& part : result._parts) {
			struct protection_applier {
				protect const& reduced_protection;

				void operator ()(slash& slash) const {
					slash -= reduced_protection.pad.value() * slash_per_pad + reduced_protection.deflect.value() * slash_per_deflect;
				}
				void operator ()(pierce& pierce) const {
					pierce -= reduced_protection.pad.value() * pierce_per_pad + reduced_protection.deflect.value() * pierce_per_deflect;
				}
				void operator ()(cleave& cleave) const {
					cleave -= reduced_protection.pad.value() * cleave_per_pad + reduced_protection.deflect.value() * cleave_per_deflect;
				}
				void operator ()(bludgeon& bludgeon) const {
					bludgeon -= reduced_protection.pad.value() * bludgeon_per_pad + reduced_protection.deflect.value() * bludgeon_per_deflect;
				}
				void operator ()(burn& burn) const {
					burn -= reduced_protection.fireproof.value() * burn_per_fireproof;
				}
				void operator ()(freeze& freeze) const {
					freeze -= reduced_protection.frostproof.value() * freeze_per_frostproof;
				}
				void operator ()(blight& blight) const {
					blight -= reduced_protection.fortify.value() * blight_per_fortify;
				}
				void operator ()(poison& poison) const {
					poison -= reduced_protection.immunize.value() * poison_per_immunize;
				}
				void operator ()(shock& shock) const {
					shock -= reduced_protection.insulate.value() * shock_per_insulate;
				}
			};
			std::visit(protection_applier{reduced_protection}, part);

			struct damage_multiplier_applier {
				resist const& resistance;
				vuln const& vulnerability;

				void operator ()(slash& slash) const {
					slash += slash * (vulnerability.slash.value() - resistance.slash.value()) / 100.0_slash_factor;
				}
				void operator ()(pierce& pierce) const {
					pierce += pierce * (vulnerability.pierce.value() - resistance.pierce.value()) / 100.0_pierce_factor;
				}
				void operator ()(cleave& cleave) const {
					cleave += cleave * (vulnerability.cleave.value() - resistance.cleave.value()) / 100.0_cleave_factor;
				}
				void operator ()(bludgeon& bludgeon) const {
					bludgeon += bludgeon * (vulnerability.bludgeon.value() - resistance.bludgeon.value()) / 100.0_bludgeon_factor;
				}
				void operator ()(burn& burn) const {
					burn += burn * (vulnerability.burn.value() - resistance.burn.value()) / 100.0_burn_factor;
				}
				void operator ()(freeze& freeze) const {
					freeze += freeze * (vulnerability.freeze.value() - resistance.freeze.value()) / 100.0_freeze_factor;
				}
				void operator ()(blight& blight) const {
					blight += blight * (vulnerability.blight.value() - resistance.blight.value()) / 100.0_blight_factor;
				}
				void operator ()(poison& poison) const {
					poison += poison * (vulnerability.poison.value() - resistance.poison.value()) / 100.0_poison_factor;
				}
				void operator ()(shock& shock) const {
					shock += shock * (vulnerability.shock.value() - resistance.shock.value()) / 100.0_shock_factor;
				}
			};
			std::visit(damage_multiplier_applier{resistance, vulnerability}, part);
		}
		return result;
	}
}
