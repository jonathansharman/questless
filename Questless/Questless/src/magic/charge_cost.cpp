//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "charge_cost.hpp"

#include "items/magic/gatestone.hpp"

namespace ql {
	bool charge_cost::can_pay() const {
		return gatestone.charge >= amount;
	}

	void charge_cost::pay() {
		gatestone.charge -= amount;
	}
}
