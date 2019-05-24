//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "damage/group.hpp"

#include "utility/visitation.hpp"

namespace ql::dmg {
	group group::with(protect const& protection, resist const& resistance, vuln const& vulnerability) const {
		group result = *this;

		bool const bypassed = result.bypass > protection.coverage ? protect{} : protection;
		for (auto& part : result._parts) {
			if (!bypassed) {
				// Apply protection if not bypassed.
				match(part,
					[&](slash& slash) {
						slash -= protection.pad.value() * slash_per_pad + protection.deflect.value() * slash_per_deflect;
					},
					[&](pierce& pierce) {
						pierce -= protection.pad.value() * pierce_per_pad + protection.deflect.value() * pierce_per_deflect;
					},
					[&](cleave& cleave) {
						cleave -= protection.pad.value() * cleave_per_pad + protection.deflect.value() * cleave_per_deflect;
					},
					[&](bludgeon& bludgeon) {
						bludgeon -= protection.pad.value() * bludgeon_per_pad + protection.deflect.value() * bludgeon_per_deflect;
					},
					[&](burn& burn) { burn -= protection.fireproof.value() * burn_per_fireproof; },
					[&](freeze& freeze) { freeze -= protection.frostproof.value() * freeze_per_frostproof; },
					[&](blight& blight) { blight -= protection.fortify.value() * blight_per_fortify; },
					[&](poison& poison) { poison -= protection.immunize.value() * poison_per_immunize; },
					[&](shock& shock) { shock -= protection.insulate.value() * shock_per_insulate; });
			}
			// Apply vulnerability and resistance.
			match(part,
				[&](slash& slash) {
					slash += slash * (vulnerability.slash.value() - resistance.slash.value()) / 100.0_slash_factor;
				},
				[&](pierce& pierce) {
					pierce += pierce * (vulnerability.pierce.value() - resistance.pierce.value()) / 100.0_pierce_factor;
				},
				[&](cleave& cleave) {
					cleave += cleave * (vulnerability.cleave.value() - resistance.cleave.value()) / 100.0_cleave_factor;
				},
				[&](bludgeon& bludgeon) {
					bludgeon += bludgeon * (vulnerability.bludgeon.value() - resistance.bludgeon.value()) / 100.0_bludgeon_factor;
				},
				[&](burn& burn) {
					burn += burn * (vulnerability.burn.value() - resistance.burn.value()) / 100.0_burn_factor;
				},
				[&](freeze& freeze) {
					freeze += freeze * (vulnerability.freeze.value() - resistance.freeze.value()) / 100.0_freeze_factor;
				},
				[&](blight& blight) {
					blight += blight * (vulnerability.blight.value() - resistance.blight.value()) / 100.0_blight_factor;
				},
				[&](poison& poison) {
					poison += poison * (vulnerability.poison.value() - resistance.poison.value()) / 100.0_poison_factor;
				},
				[&](shock& shock) {
					shock += shock * (vulnerability.shock.value() - resistance.shock.value()) / 100.0_shock_factor;
				});
		}
		return result;
	}
}
