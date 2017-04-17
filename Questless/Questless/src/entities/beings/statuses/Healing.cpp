//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "entities/beings/statuses/Healing.h"
#include "entities/beings/Being.h"

namespace questless
{
	void Healing::subupdate(Being& target)
	{
		target.heal(_magnitude, nullptr, source());
	}
}
