/**
* @file    Discharging.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the Healing status class.
*/

#include "entities/beings/statuses/Discharging.h"
#include "entities/beings/Being.h"

namespace questless
{
	void Discharging::update(Being& target)
	{
		target.lose_mana(_magnitude);
		Status::update(target);
	}
}
