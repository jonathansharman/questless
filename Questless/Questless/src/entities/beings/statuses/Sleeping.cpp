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
	void Sleeping::subupdate(Being& target)
	{
		target.health += target.stats.health_regen * Being::health_regen_asleep_factor;
		target.mana += target.stats.mana_regen * Being::mana_regen_asleep_factor;
		target.alertness += Being::alertness_rate_asleep;
		target.satiety += Being::satiety_rate_asleep;
		target.energy += Being::energy_rate_asleep;
	}
}
