//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "entities/beings/statuses/Poisoned.h"
#include "entities/beings/Being.h"

namespace questless
{
	void Poisoned::subupdate(Being& target)
	{
		dmg::Group blight = dmg::Blight{_magnitude};
		target.take_damage(blight, nullptr, source());
	}
}
