//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "items/Breakable.h"

namespace questless
{
	std::function<void(double&, double const&)> Breakable::integrity_mutator()
	{
		return [this](double& integrity, double const& new_integrity)
		{
			integrity = std::clamp(new_integrity, 0.0, durability());
		};
	}
}
