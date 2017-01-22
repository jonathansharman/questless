/**
* @file    ScreenVector.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Defines vectors in the screen space.
*/

#pragma once

#include "Vector.h"
#include "ScreenSpace.h"

namespace units
{
	using ScreenVector = Vector<ScreenSpace>;
}
