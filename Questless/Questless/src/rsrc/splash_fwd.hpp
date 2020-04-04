//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include <gsl/pointers>

namespace ql::rsrc {
	struct splash;
	using splash_ptr = gsl::not_null<splash const*>;
}
