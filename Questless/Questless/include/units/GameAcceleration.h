/**
* @file    GameAcceleration.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Defines GameAcceleration as a rate of velocity / sec.
*/

#ifndef GAME_ACCELERATION_H
#define GAME_ACCELERATION_H

#include "GameVelocity.h"

namespace units
{
	using GameAcceleration = units::Rate<GameVelocity, double>;
}

#endif
