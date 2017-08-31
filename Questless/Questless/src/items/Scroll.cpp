//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "items/Scroll.h"
#include "entities/beings/Being.h"

namespace questless
{
	std::vector<uptr<Action>> Scroll::actions()
	{
		std::vector<uptr<Action>> actions;
		if (blank()) {
			//! @todo Write?
		}
		actions.push_back(Drop::make(*this));
		actions.push_back(Throw::make(*this));
		return actions;
	}
}
