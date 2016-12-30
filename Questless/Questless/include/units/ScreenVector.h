/**
* @file    ScreenVector.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Defines vectors in the screen space.
*/

#ifndef SCREEN_VECTOR_H
#define SCREEN_VECTOR_H

#include "Vector.h"
#include "ScreenSpace.h"

namespace units
{
	using ScreenVector = Vector<ScreenSpace>;
}

#endif
