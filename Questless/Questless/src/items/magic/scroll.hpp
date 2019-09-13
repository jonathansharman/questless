//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "magic/spell.hpp"

#include "reg.hpp"

#include <optional>

namespace ql {
	//! A magic scroll for inscribing spells.
	struct scroll {
		id id;
		std::optional<magic::spell> spell;
	};

	auto make_scroll(id scroll_id, std::optional<magic::spell> spell) -> id;
}
