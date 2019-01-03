//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "aggregate.hpp"

#include "damage/protect.hpp"
#include "damage/multiplier.hpp"

namespace ql::stats {
	//! The stats of a being as a whole.
	struct being {
		aggregate a{};

		nonnegative<ql::per_tick> regen = 0_per_tick;
		ql::temperature min_temp{0.0};
		ql::temperature max_temp{0.0};
		nonnegative<ql::stealth> stealth = 0.0_stl;
		dmg::protect protect = dmg::protect::zero();
		dmg::resist resist = dmg::resist::zero();
		dmg::vuln vuln = dmg::vuln::zero();

		being() = default;

		being
			( aggregate a
			, ql::per_tick regen
			, ql::stealth stealth
			, ql::temperature min_temp
			, ql::temperature max_temp
			, dmg::protect protect
			, dmg::resist resist
			, dmg::vuln vuln
			)
			: a{a}
			, regen{regen}
			, min_temp{min_temp}
			, max_temp{max_temp}
			, stealth{stealth}
			, protect{protect}
			, resist{resist}
			, vuln{vuln}
		{}

		template <typename Archive>
		void save(Archive& archive) const {
			archive
				( CEREAL_NVP(a)
				, CEREAL_NVP(regen)
				, CEREAL_NVP(min_temp)
				, CEREAL_NVP(max_temp)
				, CEREAL_NVP(stealth)
				, CEREAL_NVP(protect)
				, CEREAL_NVP(resist)
				, CEREAL_NVP(vuln)
				);
		}

		template <typename Archive>
		void load(Archive& archive) {
			archive
				( CEREAL_NVP(a)
				, CEREAL_NVP(regen)
				, CEREAL_NVP(min_temp)
				, CEREAL_NVP(max_temp)
				, CEREAL_NVP(stealth)
				, CEREAL_NVP(protect)
				, CEREAL_NVP(resist)
				, CEREAL_NVP(vuln)
				);
		}
	};
}
