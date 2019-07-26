//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.
//! @brief Defines @p ostream and cereal-compatible serialization functions.

#pragma once

#include "quantity.hpp"

#include <ostream>

namespace cancel {
	//! Stream extraction operator for quantities.
	template <typename Rep, typename Unit>
	std::ostream& operator <<(std::ostream& out, quantity<Rep, Unit> const& q) {
		out << q.value;
		return out;
	}

	//! cereal-compatible save function for quantities.
	template <typename Archive, typename Rep, typename Unit>
	void save(Archive& archive, quantity<Rep, Unit> const& q) { archive(q.value); }

	//! cereal-compatible load function for quantities.
	template <typename Archive, typename Rep, typename Unit>
	void load(Archive& archive, quantity<Rep, Unit>& q) { archive(q.value); }
}
