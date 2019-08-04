//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "scroll.hpp"

#include "items/item.hpp"

namespace ql {
	id make_scroll(id id, std::optional<magic::spell> spell) {
		make_item(id, 0.01_mass);
		reg.assign<scroll>(id, id, std::move(spell));
		return id;
	}
}
