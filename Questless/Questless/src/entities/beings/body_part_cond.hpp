//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "bounded/dynamic_nonnegative.hpp"
#include "bounded/lazy.hpp"
#include "quantities/misc.hpp"
#include "reg.hpp"

namespace ql {
	//! Encapsulates the temporary conditions of a body part.
	struct body_part_cond {
		//! Allows a part to be used to perform actions.
		dynamic_nonnegative<action> action{};

		//! Whether this body part is immortal.
		bool immortal = false;

		//! Whether this body part is currently functioning. Parts can be disabled by damage or other effects.
		ableness ableness = ableness::enabled;
		constexpr bool enabled() const {
			return ableness == ableness::enabled;
		}
		constexpr bool disabled() const {
			return ableness == ableness::disabled;
		}

		ql::poisoning poisoning{};

		ql::decay decay{};

		ql::hypovolemia_damage hypovolemia_damage{};
	};
}
