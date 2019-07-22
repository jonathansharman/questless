//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "reg.hpp"

namespace ql {
	struct quarterstaff {
		ent id;

		void strike();
		void jab();
	};

	void make_quarterstaff(ent id);
}
