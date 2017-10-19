//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "entities/beings/abilities.hpp"
#include "entities/beings/being.hpp"
#include "agents/agent.hpp"
#include "world/region.hpp"

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
