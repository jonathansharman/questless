/**
* @file    Weapon.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the Weapon abstract base class.
*/

#include "items/weapons/Weapon.h"
#include "entities/beings/Being.h"

namespace questless
{
	void Weapon::Ready::perform(Being& actor, cont_t cont)
	{
		actor.gain_busy_time(_weapon.wind_up());
		_weapon._ready = true;
		cont(Result::success);
	}

	void Weapon::Attack::perform(Being& actor, cont_t cont)
	{
		/// @todo This. Attacking works differently for different weapons. Should there even be a default implementation?
		actor.gain_busy_time(_weapon.follow_through());
		_weapon._ready = false;
		cont(Result::success);
	}

	void Weapon::Block::perform(Being& /*actor*/, cont_t cont)
	{
		/// @todo This. Blocking also works differently for different weapons. Should there be a default implementation of blocking either?
		cont(Result::success);
	}
}
