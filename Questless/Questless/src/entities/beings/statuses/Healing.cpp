/**
* @file    Healing.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the Healing status class.
*/

#include "entities/beings/statuses/Healing.h"
#include "entities/beings/Being.h"

namespace questless
{
	void Healing::subupdate(Being& target)
	{
		target.heal(_magnitude, nullptr, source());
	}
}
