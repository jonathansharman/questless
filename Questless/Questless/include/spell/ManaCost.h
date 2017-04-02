/**
* @file    ManaCost.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*/

#pragma once

#include "agents/Cost.h"

namespace questless
{
	/// The cost of performing an action.
	////
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
