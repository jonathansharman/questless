//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "body_part_status.hpp"

#include "utility/visitation.hpp"

namespace ql {
	body_part_status_category get_category(body_part_status const& status) {
		return match(status, [](auto const& status) { return status.category; });
	}
}
