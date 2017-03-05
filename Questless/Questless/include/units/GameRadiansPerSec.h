/**
* @file    GameRadiansPerSec.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Defines GameRadiansPerSec as a rate of radians / sec.
*/

#pragma once

#include "GameVelocity.h"

namespace units
{
	using GameRadiansPerSec = Rate<GameRadians, double>;
}
