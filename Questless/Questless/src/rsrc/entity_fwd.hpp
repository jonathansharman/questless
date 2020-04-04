//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include <gsl/pointers>

namespace ql::rsrc {
	struct entity;
	using entity_ptr = gsl::not_null<entity const*>;
}
