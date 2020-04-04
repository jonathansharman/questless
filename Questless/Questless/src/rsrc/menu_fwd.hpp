//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include <gsl/pointers>

namespace ql::rsrc {
	struct menu;
	using menu_ptr = gsl::not_null<menu const*>;
}
