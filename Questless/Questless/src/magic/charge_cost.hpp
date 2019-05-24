//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "agents/cost.hpp"
#include "quantities/misc.hpp"

namespace ql {
	struct gatestone;

	//! The cost of .
	struct charge_cost : cost {
		gatestone& gatestone;
		mana amount;

		bool can_pay() const final;

		void pay() final;
	};
}
