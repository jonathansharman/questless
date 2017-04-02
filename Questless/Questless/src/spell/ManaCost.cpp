/**
* @file    ManaCost.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*/

#pragma once

#include "spell/ManaCost.h"
#include "entities/beings/Being.h"
#include "agents/Agent.h"
#include "agents/Message.h"

namespace questless
{
	Complete ManaCost::check(Being& actor, cont_t cont) const
	{
		return actor.mana >= _amount
			? cont()
			: actor.agent().send_message(std::make_unique<MessageNotEnoughMana>(_amount - actor.mana), [cont] { return Complete{}; })
			;
	}

	void ManaCost::incur(Being& actor) const
	{
		actor.mana -= _amount;
	}
}
