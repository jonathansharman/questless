//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.
//! @brief Defines @p ostream and cereal-compatible serialization functions.

#pragma once

#include "quantity.hpp"

#include <ostream>

namespace cancel {
	//! Stream extraction operator for quantities.
	template <typename Rep, typename Unit>
	auto operator <<(std::ostream& out, quantity<Rep, Unit> const& q) -> std::ostream& {
		out << q.data;
		return out;
	}

	//! cereal-compatible save function for quantities.
	template <typename Archive, typename Rep, typename Unit>
	auto save(Archive& archive, quantity<Rep, Unit> const& q) -> void { archive(q.data); }

	//! cereal-compatible load function for quantities.
	template <typename Archive, typename Rep, typename Unit>
	auto load(Archive& archive, quantity<Rep, Unit>& q) -> void { archive(q.data); }
}
