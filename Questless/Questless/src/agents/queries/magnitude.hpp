//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <variant>

namespace ql::queries::magnitude {
	struct heal {};
	struct shock {};

	using query = std::variant<heal, shock>;
}
