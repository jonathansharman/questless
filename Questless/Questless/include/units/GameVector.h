/**
* @file    GameVector.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Defines vectors in the game space.
*/

#pragma once

#include "Vector.h"
#include "GameSpace.h"

namespace units
{
	using GameVector = Vector<GameSpace>;
}
