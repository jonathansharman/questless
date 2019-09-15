//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "effect.hpp"

#include "utility/visitation.hpp"

namespace ql::effects {
	auto effect::origin() const -> tile_hex::point {
		return match(value, [](auto const& effect) { return effect.origin; });
	}

	auto effect::range() const -> pace {
		return match(value, [](auto const& effect) { return effect.range(); });
	}
}
