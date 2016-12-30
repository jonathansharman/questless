/**
* @file    GameVector.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Defines vectors in the game space.
*/

#ifndef GAME_VECTOR_H
#define GAME_VECTOR_H

#include "Vector.h"
#include "GameSpace.h"

namespace units
{
	using GameVector = Vector<GameSpace>;
}

#endif
