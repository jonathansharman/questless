//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <variant>

namespace ql::queries::count {
	struct wait_time {};

	using query = std::variant<wait_time>;
}
