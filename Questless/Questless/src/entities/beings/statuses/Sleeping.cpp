//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

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
