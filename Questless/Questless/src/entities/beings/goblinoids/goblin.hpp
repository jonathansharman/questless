//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "../body.hpp"
#include "../stats/being.hpp"

namespace ql {
	struct being;

	struct goblin {
		double transparency() const {
			return 0.5;
		}

		std::string const& description() const {
			return "The goblins are a diminutive and simpleminded humanoid race, known particularly for their bad "
				   "tempers and love of mischief. Goblins rarely form large communities, preferring small family "
				   "groups or solitude. They are remarkably sneaky, and many an unwary traveller through goblin "
				   "territory has lost a coin purse or a limb to a goblin ambush.";
		}

		body make_body(ql::id<being> owner_id) const;

		stats::being make_base_stats() const;
	};
}
