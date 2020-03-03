//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#include "arrow.hpp"

namespace ql {
	auto make_arrow(reg& reg, id arrow_id) -> id {
		make_item(reg, arrow_id, 0.1_mass);
		reg.assign<arrow>(arrow_id, &reg, arrow_id);
		return arrow_id;
	}
}
