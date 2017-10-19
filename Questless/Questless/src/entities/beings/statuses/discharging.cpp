//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "entities/beings/statuses/discharging.hpp"
#include "entities/beings/being.hpp"

namespace ql
{
	void discharging::subupdate(being& target)
	{
		target.mana -= _magnitude;
	}
}
