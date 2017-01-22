/**
* @file    Recharging.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Recharging implementation.
*/

#include "entities/beings/statuses/Recharging.h"
#include "entities/beings/Being.h"

namespace questless
{
	void Recharging::subupdate(Being& target)
	{
		target.mana += _magnitude;
	}
}
