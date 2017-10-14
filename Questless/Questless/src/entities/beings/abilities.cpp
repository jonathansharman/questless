//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "entities/beings/abilities.h"
#include "entities/beings/being.h"
#include "agents/agent.h"
#include "world/region.h"

namespace ql
{
	void abilities::update(being& being)
	{
		auto& body = being.body;

		// Flying
		_can_fly = false;
		for (wing& wing : body.wings()) {
			if (wing.health > 0.0) {
				_can_fly = true;
				break;
			}
		}
	}
}
