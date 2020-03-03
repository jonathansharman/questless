//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#include "item.hpp"

namespace ql {
	auto make_item(reg& reg, id item_id, mass mass) -> id {
		reg.assign<ql::mass>(item_id, mass);

		return item_id;
	}
}
