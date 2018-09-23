//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "aggregate.hpp"

#include "damage/protect.hpp"
#include "damage/multiplier.hpp"
#include "utility/io.hpp"

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
	};

	void to_json(nlohmann::json& j, part const& part) {
		to_json(j["a"], part.a);
		if (part.dexterity != dexterity{0.0}) to_json(j["dexterity"], part.dexterity);
		to_json(j["protect"], part.protect);
		to_json(j["resist"], part.resist);
		to_json(j["vuln"], part.vuln);
	}

	void from_json(nlohmann::json const& j, part& part) {
		from_json(j["a"], part.a);

		auto dexterity = j.find("dexterity");
		if (dexterity != j.end()) from_json(dexterity.value(), part.dexterity);
		
		from_json(j["protect"], part.protect);
		from_json(j["resist"], part.resist);
		from_json(j["vuln"], part.vuln);
	}
}
