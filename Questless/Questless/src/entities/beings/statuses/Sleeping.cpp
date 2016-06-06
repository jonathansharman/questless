/**
* @file    Sleeping.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the Sleeping status class.
*/

#include "entities/beings/statuses/Sleeping.h"
#include "entities/beings/Being.h"

namespace questless
{
	void Sleeping::update(Being& target)
	{
		target.gain_health(target.health_regen() * Being::health_regen_asleep_factor);
		target.gain_mana(target.mana_regen() * Being::mana_regen_asleep_factor);
		target.gain_alertness(Being::alertness_rate_asleep);
		target.gain_satiety(Being::satiety_rate_asleep);
		target.gain_energy(Being::energy_rate_asleep);
		Status::update(target);
	}
}
