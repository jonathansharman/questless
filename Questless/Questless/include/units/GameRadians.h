/**
* @file    GameRadians.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Defines angles in radians in the game space.
*/

#ifndef GAME_RADIANS_H
#define GAME_RADIANS_H

#include "Angle.h"
#include "GameSpace.h"

namespace units
{
	using GameRadians = Radians<GameSpace>;
}

#endif
