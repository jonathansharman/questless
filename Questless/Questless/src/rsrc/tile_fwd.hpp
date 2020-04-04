//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include <gsl/pointers>

namespace ql::rsrc {
	struct tile;
	using tile_ptr = gsl::not_null<tile const*>;
}
