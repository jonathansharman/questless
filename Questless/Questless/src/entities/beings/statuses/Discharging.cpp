//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "entities/beings/statuses/Discharging.h"
#include "entities/beings/Being.h"

namespace questless
{
	void Discharging::subupdate(Being& target)
	{
		target.mana -= _magnitude;
	}
}
