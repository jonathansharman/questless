//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "reg.hpp"

namespace ql {
	struct quarterstaff {
		id id;

		void strike();
		void jab();
	};

	auto make_quarterstaff(id quarterstaff_id) -> id;
}
