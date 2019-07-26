//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "arrow.hpp"

namespace ql {
	ent make_arrow(ent id) {
		make_item(id, 0.1_mass);
		reg.assign<arrow>(id, id);
		return id;
	}
}
