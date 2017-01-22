/**
* @file    GameVelocity.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Defines GameVelocity as a rate of vector / sec.
*/

#pragma once

#include "units/Rate.h"
#include "units/Vector.h"
#include "units/GameSeconds.h"

namespace units
{
	using GameVelocity = units::Rate<units::GameVector, double>;
}
