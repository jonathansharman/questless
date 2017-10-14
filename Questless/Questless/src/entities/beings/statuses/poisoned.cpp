//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "entities/beings/statuses/poisoned.h"
#include "entities/beings/being.h"

namespace ql
{
	void poisoned::subupdate(being& target)
	{
		dmg::group blight = dmg::blight{_magnitude};
		target.take_damage(blight, nullptr, source());
	}
}
