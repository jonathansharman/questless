//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "entities/beings/statuses/sleeping.hpp"
#include "entities/beings/being.hpp"

namespace ql
{
	void sleeping::subupdate(being& target)
	{
		for (body_part& part : target.body.parts()) { //! @todo How should body parts regen while sleeping? Percentage? Flat amount?
			part.health += target.stats.health_regen * being::health_regen_asleep_factor;
		}
		target.alertness += being::alertness_rate_asleep;
		target.satiety += being::satiety_rate_asleep;
		target.energy += being::energy_rate_asleep;
	}
}
