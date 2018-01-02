//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "magic/mana_cost.hpp"
#include "entities/beings/being.hpp"
#include "agents/agent.hpp"
#include "agents/message.hpp"

namespace ql
{
	complete mana_cost::check(being& actor, cont cont) const
	{
		return actor.mana >= _amount
			? cont()
			: actor.agent().send_message(umake<message_not_enough_mana>(_amount - actor.mana), [cont] { return complete{}; })
			;
	}

	void mana_cost::incur(being& actor) const
	{
		actor.mana -= _amount;
	}
}
