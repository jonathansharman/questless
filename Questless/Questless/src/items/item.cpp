//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "item.hpp"

namespace ql {
	void make_item(id id, mass mass) {
		reg.assign<ql::mass>(id, mass);
	}
}
