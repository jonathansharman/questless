/**
* @file    GameRadiansPerSec.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Defines GameRadiansPerSec as a rate of radians / sec.
*/

#ifndef GAME_RADIANS_PER_SEC_H
#define GAME_RADIANS_PER_SEC_H

#include "GameVelocity.h"

namespace units
{
	using GameRadiansPerSec = units::Rate<units::GameRadians, double>;
}

#endif
