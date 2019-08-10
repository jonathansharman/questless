//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

namespace ql::stats {
	//! Encapsulates a single stat, which has a base value and current value.
	template <typename T>
	struct stat {
		T base{};
		T cur{};

		//! Resets current value to base value.
		void reset() {
			cur = base;
		}

		//! Clamps the current value into [0, base].
		void clamp_zero_base() {
			cur = std::clamp(cur, T(0), base);
		}

		//! Clamps the current value into [0, infinity).
		void clamp_nonnegative() {}
	};
}
