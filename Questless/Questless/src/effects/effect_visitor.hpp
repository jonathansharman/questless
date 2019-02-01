//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "utility/visitor_pattern.hpp"

namespace ql {
	class arrow_attack_effect;
	class telescope_effect;
	class injury_effect;
	class lightning_bolt_effect;

	using effect_subtype_list = cancel::list_t<arrow_attack_effect, telescope_effect, injury_effect, lightning_bolt_effect>;

	DEFINE_VISITORS(effect, effect_subtype_list)
}
