/**
* @file    TextureRect.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Defines rectangles in the texture space.
*/

#pragma once

#include "Rect.h"
#include "TextureSpace.h"

namespace units
{
	using TextureRect = Rect<TextureSpace>;
}
