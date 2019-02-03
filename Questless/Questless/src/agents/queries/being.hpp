//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <variant>

namespace ql::queries::being {
	struct heal_target {};

	using query = std::variant<heal_target>;
}