/**
* @file    GameRadians.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Defines angles in radians in the game space.
*/

#pragma once

#include "Angle.h"
#include "GameSpace.h"

namespace units
{
	using GameRadians = Radians<GameSpace>;
}
