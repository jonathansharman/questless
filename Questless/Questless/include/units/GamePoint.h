/**
* @file    GamePoint.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Defines points in the game space.
*/

#pragma once

#include "Point.h"
#include "GameSpace.h"

namespace units
{
	using GamePoint = Point<GameSpace>;
}
