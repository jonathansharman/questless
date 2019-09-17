//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "scroll.hpp"

#include "items/item.hpp"

namespace ql {
	auto make_scroll(id scroll_id, std::optional<magic::spell> spell) -> id {
		make_item(scroll_id, 0.01_mass);
		reg.assign<scroll>(scroll_id, scroll_id, std::move(spell));
		return scroll_id;
	}
}
