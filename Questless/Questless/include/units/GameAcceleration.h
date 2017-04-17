//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "GameVelocity.h"

namespace units
{
	using GameAcceleration = units::Rate<GameVelocity, double>;
}
