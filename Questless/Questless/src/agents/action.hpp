//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

namespace ql {
	struct being;

	//! An action that a being can perform via its controlling agent.
	struct action {
		enum class result { success, failure };

		virtual ~action() = default;

		//! Causes @p actor to perform this action.
		virtual result perform(being& actor) = 0;
	};
}
