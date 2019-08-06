//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "effect.hpp"

#include "utility/visitation.hpp"

namespace ql::effects {
	tile_hex::point effect::origin() const {
		return match(value, [](auto const& effect) { return effect.origin; });
	}

	span effect::range() const {
		return match(value, [](auto const& effect) { return effect.range(); });
	}
}
