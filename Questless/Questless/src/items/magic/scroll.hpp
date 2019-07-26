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
		ent id;
		std::optional<magic::spell> spell;
	};

	ent make_scroll(ent id, std::optional<magic::spell> spell);
}
