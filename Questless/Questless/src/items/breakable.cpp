//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "items/breakable.hpp"

namespace ql {
	std::function<void(double&, double const&)> breakable::integrity_mutator() {
		return [this](double& integrity, double const& new_integrity) {
			integrity = std::clamp(new_integrity, 0.0, durability());
		};
	}
}
