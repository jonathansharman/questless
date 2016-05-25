/**
* @file    Recharging.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the Recharging status class.
*/

#include "entities/beings/statuses/Recharging.h"
#include "entities/beings/Being.h"

namespace questless
{
	void Recharging::update(Being& target)
	{
		target.gain_mana(_magnitude);
		Status::update(target);
	}
}
