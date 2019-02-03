//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "effect.hpp"

#include "all_effects.hpp"

namespace ql::effects {
	region_tile::point origin(effect const& effect) {
		return std::visit([](auto const& effect) { return effect.origin; }, effect);
	}

	span range(effect const& effect) {
		return std::visit([](auto const& effect) { return effect.range(); }, effect);
	}
}
