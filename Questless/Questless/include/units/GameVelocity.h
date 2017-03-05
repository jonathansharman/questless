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
#include "units/GameVector.h"
#include "units/GameSeconds.h"

namespace units
{
	using GameVelocity = Rate<units::GameVector, double>;
}
