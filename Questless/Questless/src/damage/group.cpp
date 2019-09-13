//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "group.hpp"
#include "damage.hpp"

#include "utility/visitation.hpp"

namespace ql::dmg {
	auto group::against(armor const& armor) const -> group {
		group result = *this;

		// Return the damage unmodified if the armor was bypassed.
		if (result.bypass > armor.coverage) { return result; }

		for (auto& part : result.parts) {
			// Apply protection if not bypassed.
			match(
				part,
				[&](slash& slash) { slash -= armor.protect.slash.value(); },
				[&](pierce& pierce) { pierce -= armor.protect.pierce.value(); },
				[&](cleave& cleave) { cleave -= armor.protect.cleave.value(); },
				[&](bludgeon& bludgeon) { bludgeon -= armor.protect.bludgeon.value(); },
				[&](scorch& scorch) { scorch -= armor.protect.scorch.value(); },
				[&](freeze& freeze) { freeze -= armor.protect.freeze.value(); },
				[&](shock& shock) { shock -= armor.protect.shock.value(); },
				[&](poison& poison) { poison -= armor.protect.poison.value(); },
				[&](rot& rot) { rot -= armor.protect.rot.value(); });
			// Apply vulnerability and resistance.
			match(
				part,
				[&](slash& slash) {
					slash += slash * (armor.vuln.slash.value() - armor.resist.slash.value()) / 100_slash_factor;
				},
				[&](pierce& pierce) {
					pierce += pierce * (armor.vuln.pierce.value() - armor.resist.pierce.value()) / 100_pierce_factor;
				},
				[&](cleave& cleave) {
					cleave += cleave * (armor.vuln.cleave.value() - armor.resist.cleave.value()) / 100_cleave_factor;
				},
				[&](bludgeon& bludgeon) {
					bludgeon += bludgeon * (armor.vuln.bludgeon.value() - armor.resist.bludgeon.value()) / 100_bludgeon_factor;
				},
				[&](scorch& scorch) {
					scorch += scorch * (armor.vuln.scorch.value() - armor.resist.scorch.value()) / 100_scorch_factor;
				},
				[&](freeze& freeze) {
					freeze += freeze * (armor.vuln.freeze.value() - armor.resist.freeze.value()) / 100_freeze_factor;
				},
				[&](shock& shock) {
					shock += shock * (armor.vuln.shock.value() - armor.resist.shock.value()) / 100_shock_factor;
				},
				[&](poison& poison) {
					poison += poison * (armor.vuln.poison.value() - armor.resist.poison.value()) / 100_poison_factor;
				},
				[&](rot& rot) { rot += rot * (armor.vuln.rot.value() - armor.resist.rot.value()) / 100_rot_factor; });
		}
		return result;
	}
}
