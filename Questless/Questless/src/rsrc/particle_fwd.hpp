//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include <gsl/pointers>

namespace ql::rsrc {
	struct particle;
	using particle_ptr = gsl::not_null<particle const*>;
}
