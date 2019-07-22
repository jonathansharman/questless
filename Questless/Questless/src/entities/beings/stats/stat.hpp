//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

namespace ql::stats {
	//! Encapsulates a single stat, which has a base value and current value.
	template <typename T>
	struct stat {
		T base;
		T cur;
	};
}
