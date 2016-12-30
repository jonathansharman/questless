/**
* @file    TextureRect.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Defines rectangles in the texture space.
*/

#ifndef TEXTURE_RECT_H
#define TEXTURE_RECT_H

#include "Rect.h"
#include "TextureSpace.h"

namespace units
{
	using TextureRect = Rect<TextureSpace>;
}

#endif
