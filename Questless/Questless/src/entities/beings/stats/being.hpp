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

		nonnegative<ql::per_tick> regen = 0.0_per_tick;
		nonnegative<ql::toughness> toughness = 0.0_tgh;
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
			, ql::toughness toughness
			, ql::stealth stealth
			, ql::temperature min_temp
			, ql::temperature max_temp
			, dmg::protect protect
			, dmg::resist resist
			, dmg::vuln vuln
			)
			: a{a}
			, regen{regen}
			, toughness{toughness}
			, min_temp{min_temp}
			, max_temp{max_temp}
			, stealth{stealth}
			, protect{protect}
			, resist{resist}
			, vuln{vuln}
		{}
	};

	void to_json(nlohmann::json& j, being const& being) {
		to_json(j["a"], being.a);
		if (being.regen != 0.0_per_tick) to_json(j["regen"], being.regen);
		if (being.toughness != 0.0_tgh) to_json(j["toughness"], being.toughness);
		to_json(j["min_temp"], being.min_temp);
		to_json(j["max_temp"], being.max_temp);
		if (being.stealth != 0.0_stl) to_json(j["stealth"], being.stealth);
		to_json(j["protect"], being.protect);
		to_json(j["resist"], being.resist);
		to_json(j["vuln"], being.vuln);
	}

	void from_json(nlohmann::json const& j, being& being) {
		from_json(j["a"], being.a);

		auto regen = j.find("regen");
		if (regen != j.end()) from_json(regen.value(), being.regen);

		auto toughness = j.find("toughness");
		if (toughness != j.end()) from_json(toughness.value(), being.toughness);

		auto min_temp = j.find("min_temp");
		if (min_temp != j.end()) from_json(min_temp.value(), being.min_temp);

		auto max_temp = j.find("max_temp");
		if (max_temp != j.end()) from_json(max_temp.value(), being.max_temp);

		auto stealth = j.find("stealth");
		if (stealth != j.end()) from_json(stealth.value(), being.stealth);

		from_json(j["protect"], being.protect);
		from_json(j["resist"], being.resist);
		from_json(j["vuln"], being.vuln);
	}
}
