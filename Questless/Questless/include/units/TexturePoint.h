/**
* @file    TexturePoint.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Defines points in the texture space.
*/

#ifndef TEXTURE_POINT_H
#define TEXTURE_POINT_H

#include "Point.h"
#include "TextureSpace.h"

namespace units
{
	using TexturePoint = Point<TextureSpace>;
}

#endif
