//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include <gsl/pointers>

namespace ql::rsrc {
	struct fonts;
	using fonts_ptr = gsl::not_null<fonts const*>;
}
