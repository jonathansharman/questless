//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "entities/beings/statuses/healing.hpp"
#include "entities/beings/being.hpp"

namespace ql
{
	void healing::subupdate(being& target)
	{
		target.heal(_magnitude, nullptr, source());
	}
}
