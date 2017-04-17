//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "agents/Cost.h"

namespace questless
{
	//! The cost of performing an action.
	class ManaCost : public Cost
	{
	public:
		ManaCost(double amount) : _amount{amount} {}

		Complete check(Being& actor, cont_t cont) const override;
		
		void incur(Being& actor) const override;
	private:
		double _amount;
	};
}
