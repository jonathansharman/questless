//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "cancel/quantity.hpp"

namespace ql {
	//! In-game distance, defined as the distance between adjacent tiles.
	using span = cancel::quantity<int, cancel::unit_t<struct span_tag>>;
	constexpr span operator "" _span(unsigned long long value) { return span{static_cast<int>(value)}; }

	//! Distance between sections.
	using section_span = cancel::quantity<int, cancel::unit_t<struct section_span_tag>>;
	constexpr section_span operator "" _section_span(unsigned long long value) { return section_span{static_cast<int>(value)}; }
}
