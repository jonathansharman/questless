//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "entities/beings/statuses/poisoned.hpp"
#include "entities/beings/being.hpp"

namespace ql {
	void poisoned::subupdate(being& target, tick elapsed) {
		for (body_part& part : target.body.parts()) {
			dmg::group damage = _rate * elapsed;
			target.take_damage(damage, part, source());
		}
	}
}
