//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "GameSpace.h"
#include "Rate.h"

namespace units
{
	using GameRadiansPerSec = Rate<GameSpace::Radians, double>;
}
