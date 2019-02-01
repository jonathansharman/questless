//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "utility/visitor_pattern.hpp"

namespace ql::magic {
	class telescope;
	class heal;
	class shock;
	class teleport;

	using spell_subtype_list = cancel::list_t<telescope, heal, shock, teleport>;

	DEFINE_VISITORS(spell, spell_subtype_list)
}
