//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "entities/beings/statuses/sleeping.h"
#include "entities/beings/being.h"

namespace ql
{
	void sleeping::subupdate(being& target)
	{
		target.health += target.stats.health_regen * being::health_regen_asleep_factor;
		target.mana += target.stats.mana_regen * being::mana_regen_asleep_factor;
		target.alertness += being::alertness_rate_asleep;
		target.satiety += being::satiety_rate_asleep;
		target.energy += being::energy_rate_asleep;
	}
}
