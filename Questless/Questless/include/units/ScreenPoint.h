/**
* @file    ScreenPoint.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Defines points in the screen space.
*/

#pragma once

#include "Point.h"
#include "ScreenSpace.h"

namespace units
{
	using ScreenPoint = Point<ScreenSpace>;
}
