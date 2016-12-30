/**
* @file    GameScaleVelocity.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Defines GameScaleVelocity as a rate of double / sec.
*/

#ifndef GAME_SCALE_VELOCITY_H
#define GAME_SCALE_VELOCITY_H

#include "GameVelocity.h"

namespace units
{
	using GameScaleVelocity = units::Rate<double, double>;
}

#endif
