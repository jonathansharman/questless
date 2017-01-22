/**
* @file    ScreenRect.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Defines rectangles in the screen space.
*/

#pragma once

#include "Rect.h"
#include "ScreenSpace.h"

namespace units
{
	using ScreenRect = Rect<ScreenSpace>;
}
