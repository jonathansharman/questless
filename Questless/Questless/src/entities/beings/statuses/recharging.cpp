//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "entities/beings/statuses/recharging.h"
#include "entities/beings/being.h"

namespace ql
{
	void recharging::subupdate(being& target)
	{
		target.mana += _magnitude;
	}
}
