//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "reg.hpp"

namespace ql {
	struct quarterstaff {
		reg_ptr reg;
		id id;

		void strike();
		void jab();
	};

	auto make_quarterstaff(reg& reg, id quarterstaff_id) -> id;
}
