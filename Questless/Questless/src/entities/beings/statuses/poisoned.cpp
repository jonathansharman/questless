//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "entities/beings/statuses/poisoned.hpp"
#include "entities/beings/being.hpp"

namespace ql {
	void poisoned::subupdate(being& target) {
		dmg::group poison{dmg::poison{_magnitude}};
		for (body_part& part : target.body.parts()) {
			target.take_damage(poison, part, source());
		}
	}
}
