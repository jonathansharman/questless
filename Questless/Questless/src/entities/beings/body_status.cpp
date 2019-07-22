//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "body_status.hpp"

#include "utility/visitation.hpp"

namespace ql {
	body_status_category get_category(body_status const& status) {
		return match(status, [](auto const& status) { return status.category; });
	}
}
