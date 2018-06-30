//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "agents/cost.hpp"

namespace ql
{
	class gatestone;

	//! The cost of performing an action.
	class charge_cost : public cost
	{
	public:
		charge_cost(gatestone& gatestone, double amount)
			: _gatestone{gatestone}
			, _amount{amount}
		{}

		complete check(being& actor, cont cont) const final;
		
		void incur(being& actor) const final;
	private:
		gatestone& _gatestone;
		double _amount;
	};
}
