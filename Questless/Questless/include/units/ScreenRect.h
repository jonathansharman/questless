/**
* @file    ScreenRect.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Defines rectangles in the screen space.
*/

#ifndef SCREEN_RECT_H
#define SCREEN_RECT_H

#include "Rect.h"
#include "ScreenSpace.h"

namespace units
{
	using ScreenRect = Rect<ScreenSpace>;
}

#endif
