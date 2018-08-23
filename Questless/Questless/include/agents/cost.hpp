//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <functional>
#include <memory>

#include "utility/complete.hpp"

namespace ql {
	class being;

	//! The cost of performing an action.
	class cost {
	public:
		using cont = std::function<complete()>;

		virtual ~cost() = default;

		//! Checks that the actor is able to pay the cost and executes the continuation if so.
		//! @param actor The being that is performing the action.
		//! @param cont The continuation function to call if the cost is satisfied.
		//! @note This does not incur the cost. To incur cost, call incur() after, or use check_and_incur() instead.
		virtual complete check(being& actor, cont cont) const = 0;
		
		//! Incurs the cost on the given actor.
		//! @param actor 
		virtual void incur(being& actor) const = 0;

		//! Checks that the actor is able to pay the cost and if so, incurs cost and then executes the continuation.
		//! @param actor The being that is performing the action.
		//! @param cont The continuation function to call if the cost is satisfied.
		complete check_and_incur(being& actor, cont cont) const {
			return check(actor, [&, this]() {
				incur(actor);
				return cont();
			});
		}
	};

	//! The trivial cost, for actions that don't require anything.
	class free : public cost {
	public:
		complete check(being&, cont cont) const final { return cont(); }

		void incur(being&) const final {}
	};
}
