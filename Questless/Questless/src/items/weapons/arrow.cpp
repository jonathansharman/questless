//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#include "arrow.hpp"

namespace ql {
	auto make_arrow(id arrow_id) -> id {
		make_item(arrow_id, 0.1_mass);
		reg.assign<arrow>(arrow_id, arrow_id);
		return arrow_id;
	}
}
