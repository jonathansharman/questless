//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "entities/beings/statuses/healing.hpp"
#include "entities/beings/being.hpp"

namespace ql
{
	void healing::subupdate(being& target)
	{
		for (body_part& part : target.body) {
			target.heal(_magnitude, part, source());
		}
	}
}
