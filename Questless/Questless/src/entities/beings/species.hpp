//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "human.hpp"
#include "stats/being.hpp"

#include "utility/visitation.hpp"

namespace ql {
	// A being's species.
	struct species {
		std::variant<human> value;

		double transparency() const {
			return match(value, [](auto const& species) { return species.transparency(); });
		}

		//! @todo Put this in being or entity?
		std::string description() const {
			return match(value, [](auto const& species) { return species.description(); });
		}

		body make_body(ql::id<being> owner_id) const {
			return match(value, [owner_id](auto const& species) { return species.make_body(owner_id); });
		}

		stats::being make_base_stats() const {
			return match(value, [](auto const& species) { return species.make_base_stats(); });
		}
	};
}
