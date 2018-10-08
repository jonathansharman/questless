//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "aggregate.hpp"

#include "damage/protect.hpp"
#include "damage/multiplier.hpp"

#include <cereal/cereal.hpp>

namespace ql::stats {
	//! Stats belonging only to a particular body part.
	struct part {
		aggregate a{};

		nonnegative<ql::dexterity> dexterity = 0.0_dex;

		dmg::protect protect = dmg::protect::zero();
		dmg::resist resist = dmg::resist::zero();
		dmg::vuln vuln = dmg::vuln::zero();

		part() = default;

		part
			( aggregate a
			, ql::dexterity dexterity
			, dmg::protect protect
			, dmg::resist resist
			, dmg::vuln vuln
			)
			: a{std::move(a)}
			, dexterity{dexterity}
			, protect{protect}
			, resist{resist}
			, vuln{vuln}
		{}

		template <typename Archive>
		void save(Archive& archive) const {
			archive
				( CEREAL_NVP(a)
				, CEREAL_NVP(dexterity)
				, CEREAL_NVP(protect)
				, CEREAL_NVP(resist)
				, CEREAL_NVP(vuln)
				);
		}

		template <typename Archive>
		void load(Archive& archive) {
			archive
				( CEREAL_NVP(a)
				, CEREAL_NVP(dexterity)
				, CEREAL_NVP(protect)
				, CEREAL_NVP(resist)
				, CEREAL_NVP(vuln)
				);
		}
	};
}
