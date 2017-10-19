//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "agents/cost.hpp"

namespace ql
{
	//! The cost of performing an action.
	class mana_cost : public cost
	{
	public:
		mana_cost(double amount) : _amount{amount} {}

		complete check(being& actor, cont cont) const final;
		
		void incur(being& actor) const final;
	private:
		double _amount;
	};
}
