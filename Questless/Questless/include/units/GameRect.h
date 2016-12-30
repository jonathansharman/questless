/**
* @file    GameRect.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Defines rectangles in the game space.
*/

#ifndef GAME_RECT_H
#define GAME_RECT_H

#include "Rect.h"
#include "GameSpace.h"

namespace units
{
	using GameRect = Rect<GameSpace>;
}

#endif
