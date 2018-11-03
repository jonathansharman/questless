//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "quantities/quantities.hpp"

#include <variant>

namespace ql::queries::tile {
	struct ranged_attack_target { span range; };
	struct shock_target {};
	struct teleport_target {};

	using any = std::variant
		< ranged_attack_target
		, shock_target
		, teleport_target
		>;
}
