//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "utility/visitor_pattern.hpp"

namespace ql::magic {
	class eagle_eye;
	class heal;
	class shock;
	class teleport;

	using spell_subtype_list = cancel::list_t
		< eagle_eye
		, heal
		, shock
		, teleport
		>;

	DEFINE_VISITORS(spell, spell_subtype_list)
}
