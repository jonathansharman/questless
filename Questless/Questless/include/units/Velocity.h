/**
* @file    Velocity.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Defines velocity in units of GameVector per double-precision floating-point second.
*/

#ifndef VELOCITY_H
#define VELOCITY_H

#include "Vector.h"
#include "Rate.h"

namespace units
{
	using GameVelocity = Rate<GameVector, double>;
}

#endif
