/**
* @file    GamePoint.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Defines points in the game space.
*/

#ifndef GAME_POINT_H
#define GAME_POINT_H

#include "Point.h"
#include "GameSpace.h"

namespace units
{
	using GamePoint = Point<GameSpace>;
}

#endif
