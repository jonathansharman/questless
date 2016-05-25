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
		target.gain_health(target.health_regen() * BeingK::health_regen_asleep_factor);
		target.gain_mana(target.mana_regen() * BeingK::mana_regen_asleep_factor);
		target.gain_alertness(BeingK::alertness_rate_asleep);
		target.gain_satiety(BeingK::satiety_rate_asleep);
		target.gain_energy(BeingK::energy_rate_asleep);
		Status::update(target);
	}
}
