/**
* @file    GameAcceleration.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Defines GameAcceleration as a rate of velocity / sec.
*/

#pragma once

#include "GameVelocity.h"

namespace units
{
	using GameAcceleration = units::Rate<GameVelocity, double>;
}
