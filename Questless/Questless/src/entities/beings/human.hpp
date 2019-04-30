//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "stats/being.hpp"

namespace ql {
	struct human {
		double transparency() const {
			return 0.5;
		}

		std::string description() const {
			return "You already know about humans.";
		}

		ql::body make_body(ql::id<being> owner_id) const;

		stats::being make_base_stats() const;
	};
}
