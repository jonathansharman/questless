/**
* @file    GameScaleVelocity.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Defines GameScaleVelocity as a rate of double / sec.
*/

#pragma once

#include "GameVelocity.h"

namespace units
{
	using GameScaleVelocity = units::Rate<double, double>;
}
