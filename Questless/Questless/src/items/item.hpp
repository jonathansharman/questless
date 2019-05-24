//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "quantities/mass.hpp"
#include "reg.hpp"

namespace ql {
	//! Makes @p id an item that can be picked up, used by beings, etc.
	void make_item(ent id, mass mass);
}
