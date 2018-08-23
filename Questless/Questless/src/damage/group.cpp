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
					slash -= reduced_protection.pad() * pct_pad_to_slash + reduced_protection.deflect() * pct_deflect_to_slash;
				}
				void operator ()(pierce& pierce) const {
					pierce -= reduced_protection.pad() * pct_pad_to_pierce + reduced_protection.deflect() * pct_deflect_to_pierce;
				}
				void operator ()(cleave& cleave) const {
					cleave -= reduced_protection.pad() * pct_pad_to_cleave + reduced_protection.deflect() * pct_deflect_to_cleave;
				}
				void operator ()(bludgeon& bludgeon) const {
					bludgeon -= reduced_protection.pad() * pct_pad_to_bludgeon + reduced_protection.deflect() * pct_deflect_to_bludgeon;
				}
				void operator ()(burn& burn) const {
					burn -= reduced_protection.fireproof() * pct_fireproof_to_burn;
				}
				void operator ()(freeze& freeze) const {
					freeze -= reduced_protection.frostproof() * pct_frostproof_to_freeze;
				}
				void operator ()(blight& blight) const {
					blight -= reduced_protection.fortify() * pct_fortify_to_blight;
				}
				void operator ()(poison& poison) const {
					poison -= reduced_protection.immunize() * pct_immunize_to_poison;
				}
				void operator ()(shock& shock) const {
					shock -= reduced_protection.insulate() * pct_insulate_to_shock;
				}
			};
			std::visit(protection_applier{reduced_protection}, part);

			struct damage_multiplier_applier {
				resist const& resistance;
				vuln const& vulnerability;

				void operator ()(slash& slash) const {
					slash = dmg::slash{std::max(0.0, slash * (1.0 + (vulnerability.slash() - resistance.slash()) / 100.0))};
				}
				void operator ()(pierce& pierce) const {
					pierce = dmg::pierce{std::max(0.0, pierce * (1.0 + (vulnerability.pierce() - resistance.pierce()) / 100.0))};
				}
				void operator ()(cleave& cleave) const {
					cleave = dmg::cleave{std::max(0.0, cleave * (1.0 + (vulnerability.cleave() - resistance.cleave()) / 100.0))};
				}
				void operator ()(bludgeon& bludgeon) const {
					bludgeon = dmg::bludgeon{std::max(0.0, bludgeon * (1.0 + (vulnerability.bludgeon() - resistance.bludgeon()) / 100.0))};
				}
				void operator ()(burn& burn) const {
					burn = dmg::burn{std::max(0.0, burn * (1.0 + (vulnerability.burn() - resistance.burn()) / 100.0))};
				}
				void operator ()(freeze& freeze) const {
					freeze = dmg::freeze{std::max(0.0, freeze * (1.0 + (vulnerability.freeze() - resistance.freeze()) / 100.0))};
				}
				void operator ()(blight& blight) const {
					blight = dmg::blight{std::max(0.0, blight * (1.0 + (vulnerability.blight() - resistance.blight()) / 100.0))};
				}
				void operator ()(poison& poison) const {
					poison = dmg::poison{std::max(0.0, poison * (1.0 + (vulnerability.poison() - resistance.poison()) / 100.0))};
				}
				void operator ()(shock& shock) const {
					shock = dmg::shock{std::max(0.0, shock * (1.0 + (vulnerability.shock() - resistance.shock()) / 100.0))};
				}
			};
			std::visit(damage_multiplier_applier{resistance, vulnerability}, part);
		}
		return result;
	}
}
