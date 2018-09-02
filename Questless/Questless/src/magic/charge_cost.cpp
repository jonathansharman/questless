//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "magic/charge_cost.hpp"

#include "agents/agent.hpp"
#include "agents/queries/message.hpp"
#include "entities/beings/being.hpp"
#include "items/magic/gatestone.hpp"

namespace ql {
	complete charge_cost::check(being& actor, cont cont) const {
		return _gatestone.charge >= _amount
			? cont()
			: actor.agent().send_message(queries::message::not_enough_charge{_amount - _gatestone.charge}, [] { return complete{}; })
			;
	}

	void charge_cost::incur(being&) const {
		_gatestone.charge -= _amount;
	}
}
