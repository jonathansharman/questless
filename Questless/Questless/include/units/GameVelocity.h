//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "units/Rate.h"
#include "units/GameVector.h"
#include "units/GameSeconds.h"

namespace units
{
	using GameVelocity = Rate<units::GameVector, double>;
}
