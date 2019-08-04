//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "arrow.hpp"

namespace ql {
	id make_arrow(id id) {
		make_item(id, 0.1_mass);
		reg.assign<arrow>(id, id);
		return id;
	}
}
