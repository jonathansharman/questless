/**
* @file    Velocity.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Defines velocity in units of GameVector per double-precision floating-point second.
*/

#pragma once

#include "Vector.h"
#include "Rate.h"

namespace units
{
	using GameVelocity = Rate<GameVector, double>;
}
