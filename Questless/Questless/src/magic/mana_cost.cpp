//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "magic/mana_cost.h"
#include "entities/beings/being.h"
#include "agents/agent.h"
#include "agents/message.h"

namespace ql
{
	complete mana_cost::check(being& actor, cont cont) const
	{
		return actor.mana >= _amount
			? cont()
			: actor.agent().send_message(std::make_unique<message_not_enough_mana>(_amount - actor.mana), [cont] { return complete{}; })
			;
	}

	void mana_cost::incur(being& actor) const
	{
		actor.mana -= _amount;
	}
}
