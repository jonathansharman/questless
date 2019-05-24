//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <functional>
#include <memory>

#include "utility/complete.hpp"

namespace ql {
	struct being;

	//! The cost of performing an action.
	struct cost {
		virtual ~cost() = default;

		//! Checks whether this cost can be paid.
		//! @note Does not incur this cost. To incur cost, call @p pay() after, or use @p check_and_pay() instead.
		[[nodiscard]] virtual bool can_pay() const = 0;

		//! Incurs this cost.
		virtual void pay() = 0;

		//! Checks that this cost can be paid and if so pays it.
		[[nodiscard]] bool check_and_pay() {
			if (can_pay()) { pay(); };
		}
	};

	//! The trivial cost, for actions that don't require anything.
	struct free : cost {
		bool can_pay() const final {
			return true;
		}

		void pay() final {}
	};
}
