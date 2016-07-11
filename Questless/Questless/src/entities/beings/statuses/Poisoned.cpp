/**
* @file    Poisoned.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the Poisoned status class.
*/

#include "entities/beings/statuses/Poisoned.h"
#include "entities/beings/Being.h"

namespace questless
{
	void Poisoned::subupdate(Being& target)
	{
		target.take_damage(Damage::from_blight(_magnitude), source());
	}
}
