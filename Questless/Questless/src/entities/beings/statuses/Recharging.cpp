//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "entities/beings/statuses/Recharging.h"
#include "entities/beings/Being.h"

namespace questless
{
	void Recharging::subupdate(Being& target)
	{
		target.mana += _magnitude;
	}
}
