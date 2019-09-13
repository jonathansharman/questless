//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "body_part_status.hpp"

#include "utility/visitation.hpp"

namespace ql {
	auto get_category(body_part_status const& status) -> body_part_status_category {
		return match(status, [](auto const& status) { return status.category; });
	}
}
