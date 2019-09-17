//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "quantities/mass.hpp"
#include "reg.hpp"

namespace ql {
	//! Makes @p item_id an item that can be picked up, used by beings, etc.
	auto make_item(id item_id, mass mass) -> id;
}
