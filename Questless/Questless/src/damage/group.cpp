//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#include "group.hpp"
#include "damage.hpp"

#include "utility/visitation.hpp"

namespace ql::dmg {
	auto group::against(armor const& armor) const -> group {
		group result = *this;

		// Return the damage unmodified if the armor was bypassed.
		if (result.bypass.get() > armor.coverage.get()) { return result; }

		for (auto& part : result.parts) {
			// Apply protection if not bypassed.
			match(
				part,
				[&](slash& slash) { slash -= armor.protect.slash.get(); },
				[&](pierce& pierce) { pierce -= armor.protect.pierce.get(); },
				[&](cleave& cleave) { cleave -= armor.protect.cleave.get(); },
				[&](bludgeon& bludgeon) { bludgeon -= armor.protect.bludgeon.get(); },
				[&](scorch& scorch) { scorch -= armor.protect.scorch.get(); },
				[&](freeze& freeze) { freeze -= armor.protect.freeze.get(); },
				[&](shock& shock) { shock -= armor.protect.shock.get(); },
				[&](poison& poison) { poison -= armor.protect.poison.get(); },
				[&](rot& rot) { rot -= armor.protect.rot.get(); });
			// Apply vulnerability and resistance.
			match(
				part,
				[&](slash& slash) {
					slash += slash * (armor.vuln.slash.get() - armor.resist.slash.get()) / 100_slash_factor;
				},
				[&](pierce& pierce) {
					pierce += pierce * (armor.vuln.pierce.get() - armor.resist.pierce.get()) / 100_pierce_factor;
				},
				[&](cleave& cleave) {
					cleave += cleave * (armor.vuln.cleave.get() - armor.resist.cleave.get()) / 100_cleave_factor;
				},
				[&](bludgeon& bludgeon) {
					bludgeon += bludgeon * (armor.vuln.bludgeon.get() - armor.resist.bludgeon.get()) / 100_bludgeon_factor;
				},
				[&](scorch& scorch) {
					scorch += scorch * (armor.vuln.scorch.get() - armor.resist.scorch.get()) / 100_scorch_factor;
				},
				[&](freeze& freeze) {
					freeze += freeze * (armor.vuln.freeze.get() - armor.resist.freeze.get()) / 100_freeze_factor;
				},
				[&](shock& shock) {
					shock += shock * (armor.vuln.shock.get() - armor.resist.shock.get()) / 100_shock_factor;
				},
				[&](poison& poison) {
					poison += poison * (armor.vuln.poison.get() - armor.resist.poison.get()) / 100_poison_factor;
				},
				[&](rot& rot) { rot += rot * (armor.vuln.rot.get() - armor.resist.rot.get()) / 100_rot_factor; });
		}
		return result;
	}
}
