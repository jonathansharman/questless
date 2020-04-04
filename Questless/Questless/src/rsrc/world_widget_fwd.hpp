//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include <gsl/pointers>

namespace ql::rsrc {
	struct world_widget;
	using world_widget_ptr = gsl::not_null<world_widget const*>;
}
