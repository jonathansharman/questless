//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "cancel/quantity.hpp"

namespace ql {
	//! In-game distance, defined as the distance between adjacent tiles.
	using pace = cancel::quantity<int, cancel::unit_t<struct pace_tag>>;
	constexpr auto operator"" _pace(unsigned long long value) {
		return pace{static_cast<int>(value)};
	}

	//! Distance between sections.
	using section_span = cancel::quantity<int, cancel::unit_t<struct section_pace_tag>>;
	constexpr auto operator"" _section_span(unsigned long long value) {
		return section_span{static_cast<int>(value)};
	}
}
