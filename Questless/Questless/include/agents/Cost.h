//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <functional>
#include <memory>

#include "utility/Complete.h"

namespace questless
{
	class Being;

	//! The cost of performing an action: mana, ammo, etc.
	class Cost
	{
	public:
		using cont_t = std::function<Complete()>;

		virtual ~Cost() = default;

		//! Checks that the actor is able to pay the cost and executes the continuation if so.
		//! @param actor The being that is performing the action.
		//! @param cont The continuation function to call if the cost is satisfied.
		//! @return A Complete object.
		virtual Complete check(Being& actor, cont_t cont) const = 0;
		
		//! Incurs the cost on the given actor.
		//! @param actor 
		virtual void incur(Being& actor) const = 0;
	};

	//! The trivial cost, for actions that don't require anything.
	class Free : public Cost
	{
	public:
		Complete check(Being&, cont_t cont) const override
		{
			return cont();
		}

		void incur(Being&) const override {}
	};
}
