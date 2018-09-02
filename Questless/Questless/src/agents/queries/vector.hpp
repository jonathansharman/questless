//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <variant>

namespace ql::queries::vector {
	struct melee_attack {};

	using any = std::variant<melee_attack>;
}
