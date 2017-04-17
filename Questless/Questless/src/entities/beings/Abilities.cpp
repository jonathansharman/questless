//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "entities/beings/Abilities.h"
#include "entities/beings/Being.h"
#include "agents/Agent.h"
#include "world/Region.h"

namespace questless
{
	void Abilities::update(Being& being)
	{
		auto& body = being.body;

		// Flying
		_can_fly = false;
		for (Wing& wing : body.wings()) {
			if (wing.health > 0.0) {
				_can_fly = true;
				break;
			}
		}
	}
}
