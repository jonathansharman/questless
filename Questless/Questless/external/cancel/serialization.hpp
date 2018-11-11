//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.
//! @brief Defines cereal-compatible serialization functions.

#pragma once

#include "quantity.hpp"

namespace cancel {
	//! cereal-compatible save function for quantities.
	template <typename Archive, typename Rep, typename Unit>
	void save(Archive& archive, quantity<Rep, Unit> const& q) { archive(q.value); }

	//! cereal-compatible load function for quantities.
	template <typename Archive, typename Rep, typename Unit>
	void load(Archive& archive, quantity<Rep, Unit>& q) { archive(q.value); }
}
