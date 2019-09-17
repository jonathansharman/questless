//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "coverage.hpp"
#include "multiplier.hpp"
#include "protect.hpp"

#include "bounded/static.hpp"

namespace ql::dmg {
	//! Represents a layer of damage protection, resistance, and vulnerability.
	struct armor {
		//! The protection this armor provides.
		protect protect{};

		//! The damage resistance this armor provides.
		resist resist{};

		//! The damage vulnerability this armor causes.
		vuln vuln{};

		//! The amount of coverage this armor provides.
		static_bounded<coverage, min_coverage, max_coverage> coverage{};
	};
}
