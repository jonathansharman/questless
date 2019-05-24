//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "cancel/quantity.hpp"

namespace ql {
	//! Determines how much light an entity blocks.
	using transparency = cancel::quantity<double, struct transparency_tag>;
}
